#include "Box.h"

#include <Magnum/Buffer.h>
#include <Magnum/Mesh.h>
#include <Magnum/SceneGraph/AbstractCamera.h>
#include <Magnum/Shaders/Phong.h>

namespace PushTheBox { namespace Game {

namespace {
    static const Color3 on = Color3::fromHsv(Deg(120.0f), 1.0f, 0.6f);
    static const Color3 off = Color3::fromHsv(Deg(0.0f), 1.0f, 0.6f);
}

Box::Box(const Vector2i& position, Type type, Object3D* parent, SceneGraph::DrawableGroup3D* drawables, SceneGraph::AnimableGroup3D* animables): Object3D(parent), SceneGraph::Drawable3D(*this, drawables), SceneGraph::Animable3D(*this, animables), position(position), type(type), color(type == Type::OnFloor ? off : on) {
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::Phong>("phong");
    mesh = SceneResourceManager::instance().get<Mesh>("box-mesh");

    translate(Math::swizzle<'x', '0', 'y'>(Vector2(position)));
    setDuration(0.375f);

    Interconnect::connect(*this, &Box::movedToTarget, *this, &Box::animateMoveFromToTarget);
    Interconnect::connect(*this, &Box::movedFromTarget, *this, &Box::animateMoveFromToTarget);
}

void Box::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D&) {
    shader->setTransformationMatrix(transformationMatrix)
          /** @todo rotationNormalized() when precision problems are fixed */
          .setNormalMatrix(transformationMatrix.rotationScaling())
          .setDiffuseColor(color);

    mesh->draw(*shader);
}

void Box::animationStep(Float time, Float) {
    if(type == Type::OnFloor)
        color = Math::lerp(on, off, time/duration());
    else
        color = Math::lerp(off, on, time/duration());
}

void Box::animationStopped() {
    color = type == Type::OnFloor ? off : on;
}

}}
