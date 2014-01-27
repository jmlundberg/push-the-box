#include "MeshResourceLoader.h"

#include <sstream>
#include <Corrade/Utility/Resource.h>
#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/Resource.h>
#include <Magnum/Shaders/Phong.h>

namespace PushTheBox { namespace ResourceManagement {

MeshResourceLoader::MeshResourceLoader() {
    /* Get data from compiled-in resource */
    /** @todo Configuration directly from string */
    Utility::Resource rs("PushTheBoxResources");
    std::istringstream in(rs.get("push-the-box.conf"));
    conf = new Utility::Configuration(in, Utility::Configuration::Flag::ReadOnly);
    data = rs.getRaw("push-the-box.mesh");

    /* Fill name map */
    for(std::size_t i = 0, end = conf->groupCount("mesh"); i != end; ++i)
        nameMap[conf->group("mesh", i)->value("name")] = i;
}

std::string MeshResourceLoader::name(ResourceKey key) const {
    auto it = nameMap.find(key);
    if(it == nameMap.end()) return "";
    return conf->group("mesh", it->second)->value("name");
}

void MeshResourceLoader::doLoad(ResourceKey key) {
    auto it = nameMap.find(key);
    Utility::ConfigurationGroup* group;
    if(it == nameMap.end() || !(group = conf->group("mesh", it->second))) {
        Warning() << "Resource" << key << "('" + name(key) + "') was not found";
        setNotFound(key);
        return;
    }

    /* Indexed mesh */
    Mesh* mesh = new Mesh;
    if(group->hasValue("indexOffset")) {

        /* Add index buffer to the manager */
        Buffer* indexBuffer = new Buffer(Buffer::Target::ElementArray);
        SceneResourceManager::instance().set(group->value("name") + "-index", indexBuffer, ResourceDataState::Final, ResourcePolicy::Resident);

        /* Configure indices */
        Int indexCount = group->value<Int>("indexCount");
        Mesh::IndexType indexType = group->value<Mesh::IndexType>("indexType");
        mesh->setIndexCount(indexCount)
            .setIndexBuffer(*indexBuffer, 0, indexType,
                group->value<UnsignedInt>("indexStart"), group->value<UnsignedInt>("indexEnd"));
        indexBuffer->setData({data.begin()+group->value<std::size_t>("indexOffset"),
            indexCount*Mesh::indexSize(indexType)},
            BufferUsage::StaticDraw);

    /* Non-indexed mesh */
    } else mesh = new Mesh;

    /* Add vertex buffer to the manager */
    Buffer* vertexBuffer = new Buffer;
    SceneResourceManager::instance().set(group->value("name") + "-vertex", vertexBuffer, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Configure vertices */
    mesh->setPrimitive(group->value<MeshPrimitive>("primitive"))
        .setVertexCount(group->value<Int>("vertexCount"))
        .addVertexBuffer(*vertexBuffer, 0,
            Shaders::Phong::Position(),
            Shaders::Phong::Normal(Shaders::Phong::Normal::DataType::Byte, Shaders::Phong::Normal::DataOption::Normalized),
            1);
    vertexBuffer->setData({data.begin()+group->value<std::size_t>("vertexOffset"),
                          mesh->vertexCount()*group->value<std::size_t>("vertexStride")},
                          BufferUsage::StaticDraw);

    /* Finally add the mesh to the manager */
    set(key, mesh, ResourceDataState::Final, ResourcePolicy::Resident);
}

}}
