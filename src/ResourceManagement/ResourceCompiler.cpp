#include "ResourceCompiler.h"

#include <algorithm>
#include <ostream>
#include <Magnum/Math/Functions.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/MeshTools/CompressIndices.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/Tipsify.h>
#include <Magnum/MeshTools/Transform.h>
#include <Magnum/Trade/MeshData3D.h>

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

            Containers::Array<char> indexData;
            Mesh::IndexType indexType;
            UnsignedInt indexStart, indexEnd;
            std::tie(indexData, indexType, indexStart, indexEnd) = MeshTools::compressIndices(mesh->indices());

            group->addValue("indexOffset", std::size_t(out.tellp()));
            group->addValue("indexCount", mesh->indices().size());
            group->addValue("indexType", indexType);
            group->addValue("indexStart", indexStart);
            group->addValue("indexEnd", indexEnd);

            out.write(indexData, indexData.size());
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
        const Containers::Array<char> data = MeshTools::interleave(mesh->positions(0), normals, 1);

        group->addValue("vertexArray", "3D interleaved position normal");
        group->addValue("vertexOffset", std::size_t(out.tellp()));
        group->addValue("vertexCount", mesh->positions(0).size());
        group->addValue("vertexStride", sizeof(Vector3) + sizeof(Math::Vector3<Byte>) + 1);

        out.write(data, data.size());
    }
}

}}
