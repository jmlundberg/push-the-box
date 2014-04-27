#include "FloorTile.h"

#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Phong.h>

namespace PushTheBox { namespace Game {

FloorTile::FloorTile(const Vector2i& position, Type type, Object3D* parent, SceneGraph::DrawableGroup3D* group): Object3D(parent), SceneGraph::Drawable3D(*this, group), type(type) {
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::Phong>("phong");
    mesh = SceneResourceManager::instance().get<Mesh>(ResourceKey(type == Type::Floor ? "floor-mesh" : "floor-target-mesh"));

    translate(Math::swizzle<'x', '0', 'y'>(Vector2(position)));
}

void FloorTile::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera3D&) {
    shader->setTransformationMatrix(transformationMatrix)
          /** @todo rotationNormalized() when precision problems are fixed */
          .setNormalMatrix(transformationMatrix.rotationScaling())
          .setDiffuseColor(type == Type::Floor ? Color3::fromHSV(Deg(60.0f), 0.1f, 0.8f) :
                                                 Color3::fromHSV(Deg(120.0f), 1.0f, 0.6f));

    mesh->draw(*shader);
}

}}
