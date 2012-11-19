#include "Player.h"

#include <IndexedMesh.h>
#include <MeshTools/CompressIndices.h>
#include <MeshTools/Interleave.h>
#include <Primitives/Capsule.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox {

Player::Player(Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    buffer = SceneResourceManager::instance()->get<Buffer>("player");
    indexBuffer = SceneResourceManager::instance()->get<Buffer>("playerIndices");

    /* Create player mesh, if not already exists */
    if(!(mesh = SceneResourceManager::instance()->get<Mesh, IndexedMesh>("player"))) {
        SceneResourceManager::instance()->set<Buffer>(buffer.key(),
            new Buffer, ResourceDataState::Final, ResourcePolicy::ReferenceCounted);
        /* Must explicitly set target, otherwise NaCl spits out an error */
        SceneResourceManager::instance()->set<Buffer>(indexBuffer.key(),
            new Buffer(Buffer::Target::ElementArray), ResourceDataState::Final, ResourcePolicy::ReferenceCounted);
        SceneResourceManager::instance()->set<Mesh>(mesh.key(),
            new IndexedMesh, ResourceDataState::Final, ResourcePolicy::Manual);

        Primitives::Capsule capsule(8, 1, 16, 2.0f);
        MeshTools::compressIndices(mesh, indexBuffer, Buffer::Usage::StaticDraw, *capsule.indices());
        MeshTools::interleave(mesh, buffer, Buffer::Usage::StaticDraw, *capsule.positions(0), *capsule.normals(0));
        mesh->setPrimitive(capsule.primitive())
            ->addInterleavedVertexBuffer(buffer, 0, Shaders::PhongShader::Position(), Shaders::PhongShader::Normal());
    }

    translate(Vector3::yAxis(2.0f));
    scale(Vector3(0.3f));
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformation(transformationMatrix)
          ->setProjection(camera->projectionMatrix())
          ->setDiffuseColor(Color3<GLfloat>::fromHSV(210.0f, 0.85f, 0.8f))
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}
