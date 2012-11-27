#include "FloorTile.h"

#include <Math/Constants.h>
#include <Mesh.h>
#include <Swizzle.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/PhongShader.h>

namespace PushTheBox { namespace Game {

FloorTile::FloorTile(const Math::Vector2<int>& position, Type type, Object3D* parent, SceneGraph::DrawableGroup<3>* group): Object3D(parent), SceneGraph::Drawable<3>(this, group), type(type) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::PhongShader>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>(ResourceKey(type == Type::Floor ? "floor-mesh" : "floor-target-mesh"));

    translate(Vector3::from(swizzle<'x', '0', 'y'>(position)));
}

void FloorTile::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setProjectionMatrix(camera->projectionMatrix())
          ->setDiffuseColor(type == Type::Floor ? Color3<>::fromHSV(60.0f, 0.1f, 0.8f) :
                                                  Color3<>::fromHSV(120.0f, 1.0f, 0.6f))
          ->use();

    mesh->draw();
}

}}
