#include "MeshResourceLoader.h"

#include <Utility/Resource.h>
#include <Buffer.h>
#include <IndexedMesh.h>
#include <Shaders/PhongShader.h>

#include <Resource.h>

using Corrade::Utility::Configuration;
using Corrade::Utility::ConfigurationGroup;

namespace PushTheBox { namespace ResourceManagement {

MeshResourceLoader::MeshResourceLoader() {
    /* Get data from compiled-in resource */
    /** @todo Configuration directly from string, data as raw pointer */
    Corrade::Utility::Resource rs("PushTheBoxData");
    std::istringstream in(rs.get("push-the-box.conf"));
    conf = new Configuration(in, Configuration::Flag::ReadOnly);
    data = rs.get("push-the-box.mesh");

    /* Fill name map */
    for(std::size_t i = 0, end = conf->groupCount("mesh"); i != end; ++i)
        nameMap[conf->group("mesh", i)->value("name")] = i;
}

std::string MeshResourceLoader::name(ResourceKey key) const {
    auto it = nameMap.find(key);
    if(it == nameMap.end()) return "";
    return conf->group("mesh", it->second)->value("name");
}

void MeshResourceLoader::load(ResourceKey key) {
    AbstractResourceLoader<Mesh>::load(key);

    auto it = nameMap.find(key);
    ConfigurationGroup* group;
    if(it == nameMap.end() || !(group = conf->group("mesh", it->second))) {
        Warning() << "Resource" << key << "('" + name(key) + "') was not found";
        setNotFound(key);
        return;
    }

    /* Indexed mesh */
    Mesh* mesh;
    if(group->keyExists("indexOffset")) {
        IndexedMesh* indexedMesh = new IndexedMesh;
        mesh = indexedMesh;

        /* Add index buffer to the manager */
        Buffer* indexBuffer = new Buffer(Buffer::Target::ElementArray);
        SceneResourceManager::instance()->set(group->value("name") + "-index", indexBuffer, ResourceDataState::Final, ResourcePolicy::Resident);

        /* Configure indices */
        indexedMesh->setIndexBuffer(indexBuffer)
                   ->setIndexCount(group->value<GLsizei>("indexCount"))
                   ->setIndexType(group->value<Type>("indexType"));
        indexBuffer->setData(indexedMesh->indexCount()*TypeInfo::sizeOf(indexedMesh->indexType()),
                             data.c_str()+group->value<std::size_t>("indexOffset"),
                             Buffer::Usage::StaticDraw);

    /* Non-indexed mesh */
    } else mesh = new Mesh;

    /* Add vertex buffer to the manager */
    Buffer* vertexBuffer = new Buffer;
    SceneResourceManager::instance()->set(group->value("name") + "-vertex", vertexBuffer, ResourceDataState::Final, ResourcePolicy::Resident);

    /* Configure vertices */
    mesh->setPrimitive(group->value<Mesh::Primitive>("primitive"))
        ->setVertexCount(group->value<GLsizei>("vertexCount"))
        ->addInterleavedVertexBuffer(vertexBuffer, 0, Shaders::PhongShader::Position(), Shaders::PhongShader::Normal());
    vertexBuffer->setData(mesh->vertexCount()*group->value<std::size_t>("vertexStride"),
                          data.c_str()+group->value<std::size_t>("vertexOffset"),
                          Buffer::Usage::StaticDraw);

    /* Finally add the mesh to the manager */
    set(key, mesh, ResourceDataState::Final, ResourcePolicy::Resident);
}

}}
