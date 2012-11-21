#include "Box.h"

#include <Buffer.h>
#include <Mesh.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

Box::Box(Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    /* Get shader, buffer and mesh (created in WallBrick) */
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    buffer = SceneResourceManager::instance()->get<Buffer>("cube");
    mesh = SceneResourceManager::instance()->get<Mesh>("cube");

    scale(Vector3(0.66f));
}

void Box::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformation(transformationMatrix)
          ->setProjection(camera->projectionMatrix())
          ->setDiffuseColor(Color3<GLfloat>::fromHSV(45.0f, 0.2f, 0.8f))
          ->setLightPosition((camera->cameraMatrix()*Point3D(0.5f, 2.0f, 0.5f)).xyz())
          ->use();

    mesh->draw();
}

}}
