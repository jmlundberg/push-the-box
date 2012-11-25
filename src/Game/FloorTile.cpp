#include "FloorTile.h"

#include <Math/Constants.h>
#include <Mesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

FloorTile::FloorTile(const Color3<>& color, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), color(color) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("floor-mesh");
}

void FloorTile::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(color)
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
