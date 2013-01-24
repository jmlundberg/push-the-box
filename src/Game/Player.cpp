#include "Player.h"

#include <Mesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

#include "Level.h"

namespace PushTheBox { namespace Game {

Player::Player(Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("player-mesh");
    bodyMesh = SceneResourceManager::instance()->get<Mesh>("player-body-mesh");
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>*) {
    shader->setTransformationMatrix(transformationMatrix)
          ->use();

    shader->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f));
    mesh->draw();
    shader->setDiffuseColor(Color3<>::fromHSV(50.0f, 0.85f, 0.8f));
    bodyMesh->draw();
}

}}

