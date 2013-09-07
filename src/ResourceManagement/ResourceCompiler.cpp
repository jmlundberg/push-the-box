#include "ResourceCompiler.h"

#include <algorithm>
#include <ostream>
#include <Math/Functions.h>
#include <Math/Matrix4.h>
#include <MeshTools/CompressIndices.h>
#include <MeshTools/Interleave.h>
#include <MeshTools/Tipsify.h>
#include <MeshTools/Transform.h>
#include <Trade/MeshData3D.h>

#include "configure.h"

namespace PushTheBox { namespace ResourceManagement {

ResourceCompiler::ResourceCompiler(const std::string& filename): manager(MAGNUM_PLUGINS_IMPORTER_DIR) {
    manager.load("ColladaImporter");
    importer = manager.instance("ColladaImporter");
    CORRADE_ASSERT(importer, "Cannot load ColladaImporter plugin from" << manager.pluginDirectory() << "because" << manager.loadState("ColladaImporter"), );
    CORRADE_ASSERT(importer->openFile(filename), "Cannot open file" << filename, );
}

void ResourceCompiler::compileMeshes(Utility::ConfigurationGroup* configuration, std::ostream& out) {
    for(std::size_t i = 0; i != importer->mesh3DCount(); ++i) {
        Utility::ConfigurationGroup* group = configuration->addGroup("mesh");

        /* Import mesh */
        std::optional<Trade::MeshData3D> mesh = importer->mesh3D(i);
        CORRADE_ASSERT(mesh->normalArrayCount() == 1, "Mesh" << importer->mesh3DName(i) << "has no normal array", );

        group->addValue("name", importer->mesh3DName(i));
        group->addValue("primitive", mesh->primitive());

        /* Compile index array, if present */
        if(mesh->isIndexed()) {
            /* Optimize indices */
            MeshTools::tipsify(mesh->indices(), mesh->positions(0).size(), 24);

            std::size_t indexCount;
            Mesh::IndexType indexType;
            char* data;
            std::tie(indexCount, indexType, data) = MeshTools::compressIndices(mesh->indices());
            auto minmax = std::minmax_element(mesh->indices().begin(), mesh->indices().end());

            group->addValue("indexOffset", std::size_t(out.tellp()));
            group->addValue("indexCount", indexCount);
            group->addValue("indexType", indexType);
            group->addValue("indexStart", *minmax.first);
            group->addValue("indexEnd", *minmax.second);

            out.write(data, indexCount*Mesh::indexSize(indexType));
            delete[] data;
        }

        /* Rotate to have Y up */
        /** @todo Fix this in Collada importer itself */
        auto rotation = Quaternion::rotation(-90.0_degf, Vector3::xAxis());
        MeshTools::transformVectorsInPlace(rotation, mesh->normals(0));
        MeshTools::transformVectorsInPlace(rotation, mesh->positions(0));

        /* Compress normals */
        std::vector<Math::Vector3<Byte>> normals(mesh->normals(0).size());
        std::transform(mesh->normals(0).begin(), mesh->normals(0).end(), normals.begin(),
                       [](const Vector3& vec) { return Math::denormalize<Math::Vector3<Byte>>(vec); });

        /* Compile vertex array */
        char* data;
        std::size_t vertexCount;
        std::size_t stride;
        std::tie(vertexCount, stride, data) = MeshTools::interleave(mesh->positions(0), normals, 1);

        group->addValue("vertexArray", "3D interleaved position normal");
        group->addValue("vertexOffset", std::size_t(out.tellp()));
        group->addValue("vertexCount", vertexCount);
        group->addValue("vertexStride", stride);

        out.write(data, vertexCount*stride);
        delete[] data;
    }
}

}}
