#include "WallBrick.h"

#include <Mesh.h>
#include <Swizzle.h>
#include <Shaders/Phong.h>

namespace PushTheBox { namespace Game {

WallBrick::WallBrick(const Vector2i& position, Object3D* parent, SceneGraph::DrawableGroup3D* group): Object3D(parent), SceneGraph::Drawable3D(*this, group) {
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::Phong>("phong");
    mesh = SceneResourceManager::instance().get<Mesh>("wall-mesh");

    translate(Vector3(swizzle<'x', '0', 'y'>(position)));
}

void WallBrick::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera3D&) {
    shader->setTransformationMatrix(transformationMatrix)
          /** @todo rotationNormalized() when precision problems are fixed */
          .setNormalMatrix(transformationMatrix.rotationScaling())
          .setDiffuseColor(Color3::fromHSV(Deg(30.0f), 0.2f, 1.0f))
          .use();

    mesh->draw();
}

}}
