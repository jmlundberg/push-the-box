#include "Player.h"

#include <IndexedMesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

Player::Player(Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("player-mesh");
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f))
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
