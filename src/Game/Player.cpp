#include "Player.h"

#include <IndexedMesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

#include "FieldType.h"

namespace PushTheBox { namespace Game {

Player::Player(Object3D* parent, SceneGraph::DrawableGroup<3>* group, Level *level): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("player-mesh");

    actualLevel = level;
    levelPosition = Math::Vector2<int>(0,0);
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f))
          ->use();

    mesh->draw();
}

void Player::move(int dirX, int dirY) {
    int newX = levelPosition.x() + dirX;
    int newY = levelPosition.y() + dirY;
    if(newX >= 0 && newX < actualLevel->level.size() && newY >= 0 && newY < actualLevel->level[0].size()) {
        if(actualLevel->level[newX][newY] == FieldType::Empty) {
            translate(Vector3(float(dirX), 0, float(dirY)));
            levelPosition = Math::Vector2<int>(newX, newY);
        }
    }
}

}}

