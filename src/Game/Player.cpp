#include "Player.h"

#include <IndexedMesh.h>
#include <Swizzle.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

#include "FieldType.h"
#include "Box.h"

namespace PushTheBox { namespace Game {

Player::Player(Level *level, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("player-mesh");
    bodyMesh = SceneResourceManager::instance()->get<Mesh>("player-body-mesh");

    actualLevel = level;
    levelPosition = Math::Vector2<int>(0,0);
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->use();

    shader->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f));
    mesh->draw();
    shader->setDiffuseColor(Color3<>::fromHSV(50.0f, 0.85f, 0.8f));
    bodyMesh->draw();
}

void Player::move(const Math::Vector2<int>& direction) {
    Math::Vector2<int> newPosition = levelPosition + direction;
    if(newPosition >= Math::Vector2<int>() && newPosition < Math::Vector2<int>(actualLevel->width(), actualLevel->height())) {
        if(actualLevel->value(newPosition) == FieldType::Empty || actualLevel->value(newPosition)== FieldType::Target) {
            translate(Vector3::from(swizzle<'x', '0', 'y'>(direction)));
            levelPosition = newPosition;
        }
        else if(actualLevel->value(newPosition) == FieldType::Box) {
            Math::Vector2<int> newBoxPosition = levelPosition + direction*2;
            if(newBoxPosition >= Math::Vector2<int>() && newBoxPosition < Math::Vector2<int>(actualLevel->width(), actualLevel->height()))
            {
                if(actualLevel->value(newBoxPosition) == FieldType::Empty || actualLevel->value(newBoxPosition)== FieldType::Target) {
                    translate(Vector3::from(swizzle<'x', '0', 'y'>(direction)));
                    levelPosition = newPosition;

                    actualLevel->moveBox(newPosition, newBoxPosition);
                }
            }
        }
    }
}

}}

