#include "WallTile.h"

#include <Buffer.h>
#include <Mesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

WallTile::WallTile(const Color3<GLfloat>& color, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), color(color) {
    /* Get shader, buffer and mesh (created in FloorTile class) */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    buffer = SceneResourceManager::instance()->get<Buffer>("tile");
    mesh = SceneResourceManager::instance()->get<Mesh>("tile");

    scale(Vector3(0.5f));
    translate({0.0f, 0.5f, 0.5f});
}

void WallTile::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformation(transformationMatrix)
          ->setProjection(camera->projectionMatrix())
          ->setDiffuseColor(color)
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
