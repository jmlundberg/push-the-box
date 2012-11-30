#include "Player.h"

#include <IndexedMesh.h>
#include <Swizzle.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

#include "Level.h"

namespace PushTheBox { namespace Game {

Player::Player(const Math::Vector2<int>& position, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), position(position) {
    /* Get shader and mesh buffer */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("player-mesh");
    bodyMesh = SceneResourceManager::instance()->get<Mesh>("player-body-mesh");

    translate(Vector3::from(swizzle<'x', '0', 'y'>(position)));
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>*) {
    shader->setTransformationMatrix(transformationMatrix)
          ->use();

    shader->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f));
    mesh->draw();
    shader->setDiffuseColor(Color3<>::fromHSV(50.0f, 0.85f, 0.8f));
    bodyMesh->draw();
}

void Player::move(const Math::Vector2<int>& direction) {
    Math::Vector2<int> newPosition = position + direction;

    /* Cannot move out of map */
    if(!(newPosition >= Math::Vector2<int>() && newPosition < Level::current()->size()))
        return;

    /* Pushing box */
    if(Level::current()->at(newPosition) == Level::TileType::Box ||
       Level::current()->at(newPosition) == Level::TileType::BoxOnTarget) {
        Math::Vector2<int> newBoxPosition = position + direction*2;

        /* Cannot push box out of map */
        if(!(newBoxPosition >= Math::Vector2<int>() && newBoxPosition < Level::current()->size()))
            return;

        /* The box can be pushed only on the floor */
        if(Level::current()->at(newBoxPosition) != Level::TileType::Floor &&
           Level::current()->at(newBoxPosition) != Level::TileType::Target)
            return;

        /* Move the box */
        Box* box = Level::current()->boxAt(newPosition);
        CORRADE_INTERNAL_ASSERT(box);
        box->translate(Vector3::from(swizzle<'x', '0', 'y'>(direction)));
        box->position += direction;

        if(Level::current()->at(newPosition) == Level::TileType::BoxOnTarget){
            Level::current()->at(newPosition) = Level::TileType::Target;
            ++Level::current()->targetsRemain;
        }
        else Level::current()->at(newPosition) = Level::TileType::Floor;

        if(Level::current()->at(newBoxPosition) == Level::TileType::Target)  {
            Level::current()->at(newBoxPosition) = Level::TileType::BoxOnTarget;
            --Level::current()->targetsRemain;
            box->type = Box::Type::OnTarget;
        } else {
            Level::current()->at(newBoxPosition) = Level::TileType::Box;
            box->type = Box::Type::OnFloor;
        }

    /* Other than that we can move on the floor, but nowhere else */
    } else if(Level::current()->at(newPosition) != Level::TileType::Floor &&
              Level::current()->at(newPosition) != Level::TileType::Target)
        return;

    /* Move the player */
    translate(Vector3::from(swizzle<'x', '0', 'y'>(direction)));
    position = newPosition;
}

void Player::reset(const Math::Vector2<int>& position) {
    this->position = position;
    setTransformation(Matrix4::translation(Vector3::from(swizzle<'x', '0', 'y'>(position))));
}

}}

