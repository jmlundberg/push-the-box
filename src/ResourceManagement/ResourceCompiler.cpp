#include "ResourceCompiler.h"

#include <ostream>
#include <Math/Constants.h>
#include <Math/Matrix4.h>
#include <MeshTools/CompressIndices.h>
#include <MeshTools/Interleave.h>
#include <MeshTools/Tipsify.h>
#include <MeshTools/Transform.h>
#include <Trade/MeshData3D.h>

#include "configure.h"

using Corrade::Utility::ConfigurationGroup;

namespace PushTheBox { namespace ResourceManagement {

ResourceCompiler::ResourceCompiler(const std::string& filename): manager(MAGNUM_PLUGINS_IMPORTER_DIR) {
    manager.load("ColladaImporter");
    importer = manager.instance("ColladaImporter");
    CORRADE_ASSERT(importer, "Cannot load ColladaImporter plugin from" << manager.pluginDirectory() << "because" << manager.loadState("ColladaImporter"), );
    CORRADE_ASSERT(importer->open(filename), "Cannot open file" << filename, );
}

void ResourceCompiler::compileMeshes(ConfigurationGroup* configuration, std::ostream& out) {
    for(std::size_t i = 0; i != importer->mesh3DCount(); ++i) {
        ConfigurationGroup* group = configuration->addGroup("mesh");

        /* Import mesh */
        Trade::MeshData3D* mesh = importer->mesh3D(i);
        CORRADE_ASSERT(mesh->positions(0), "Mesh" << mesh->name() << "has no position array", );
        CORRADE_ASSERT(mesh->normals(0), "Mesh" << mesh->name() << "has no normal array", );

        group->addValue("name", mesh->name());
        group->addValue("primitive", mesh->primitive());

        /* Compile index array, if present */
        if(mesh->indices()) {
            /* Optimize indices */
            MeshTools::tipsify(*mesh->indices(), mesh->positions(0)->size(), 24);

            std::size_t indexCount;
            Type indexType;
            char* data;
            std::tie(indexCount, indexType, data) = MeshTools::compressIndices(*mesh->indices());

            group->addValue("indexOffset", std::size_t(out.tellp()));
            group->addValue("indexCount", indexCount);
            group->addValue("indexType", indexType);

            out.write(data, indexCount*TypeInfo::sizeOf(indexType));
            delete[] data;
        }

        /* Rotate to have Y up */
        /** @todo Fix this in Collada importer itself */
        Matrix4 rotation = Matrix4::rotationX(deg(-90.0f));
        MeshTools::transform(rotation, *mesh->positions(0));
        MeshTools::transform(rotation.rotation(), *mesh->normals(0));

        /* Compile vertex array */
        char* data;
        std::size_t vertexCount;
        std::size_t stride;
        std::tie(vertexCount, stride, data) = MeshTools::interleave(*mesh->positions(0), *mesh->normals(0));

        group->addValue<std::string>("vertexArray", "3D interleaved position normal");
        group->addValue("vertexOffset", std::size_t(out.tellp()));
        group->addValue("vertexCount", vertexCount);
        group->addValue("vertexStride", stride);

        out.write(data, vertexCount*stride);
        delete[] data;
    }
}

}}
