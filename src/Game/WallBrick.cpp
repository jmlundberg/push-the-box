#include "WallBrick.h"

#include <Magnum/Mesh.h>
#include <Magnum/Shaders/Phong.h>

namespace PushTheBox { namespace Game {

WallBrick::WallBrick(const Vector2i& position, Object3D* parent, SceneGraph::DrawableGroup3D* group): Object3D(parent), SceneGraph::Drawable3D(*this, group) {
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::Phong>("phong");
    mesh = SceneResourceManager::instance().get<Mesh>("wall-mesh");

    translate(Math::swizzle<'x', '0', 'y'>(Vector2(position)));
}

void WallBrick::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D&) {
    shader->setTransformationMatrix(transformationMatrix)
          /** @todo rotationNormalized() when precision problems are fixed */
          .setNormalMatrix(transformationMatrix.rotationScaling())
          .setDiffuseColor(Color3::fromHsv(Deg(30.0f), 0.2f, 1.0f));

    mesh->draw(*shader);
}

}}
