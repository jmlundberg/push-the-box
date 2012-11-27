#include "Box.h"

#include <Buffer.h>
#include <Mesh.h>
#include <Swizzle.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

Box::Box(const Math::Vector2<int>& position, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("box-mesh");

    move(position);
}

void Box::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(Color3<>::fromHSV(0.0f, 1.0f, 0.6f))
          ->use();

    mesh->draw();
}

void Box::move(const Math::Vector2<int>& direction) {
    translate(Vector3::from(swizzle<'x', '0', 'y'>(direction)));
    _position += direction;
}

}}
