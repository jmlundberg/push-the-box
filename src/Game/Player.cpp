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
    level_position = Math::Vector2<int>(0,0);
}

void Player::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(Color3<>::fromHSV(210.0f, 0.85f, 0.8f))
          ->use();

    mesh->draw();
}

void Player::move(int dir_x, int dir_y){
    int new_x = level_position.x() + dir_x;
    int new_y = level_position.y() + dir_y;
    if((new_x >= 0) && (new_x < actualLevel->level.size()) && (new_y>=0) && (new_y < actualLevel->level[0].size())){
        if(actualLevel->level[new_x][new_y]==EMPTY){
            translate(Vector3(float(dir_x),0,float(dir_y)));
            level_position=Math::Vector2<int>(new_x, new_y);
        }
    }
}

}}
