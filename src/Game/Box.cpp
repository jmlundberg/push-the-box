#include "Box.h"

#include <Buffer.h>
#include <Mesh.h>
#include <Swizzle.h>
#include <SceneGraph/AbstractCamera.h>
#include <Shaders/Phong.h>

namespace PushTheBox { namespace Game {

namespace {
    static const Color3<> on = Color3<>::fromHSV(Deg(120.0f), 1.0f, 0.6f);
    static const Color3<> off = Color3<>::fromHSV(Deg(0.0f), 1.0f, 0.6f);
}

Box::Box(const Vector2i& position, Type type, Object3D* parent, SceneGraph::DrawableGroup<3>* drawables, SceneGraph::AnimableGroup<3>* animables): Object3D(parent), SceneGraph::Drawable<3>(this, drawables), SceneGraph::Animable<3>(this, animables), position(position), type(type), color(type == Type::OnFloor ? off : on) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::Phong>("phong");
    mesh = SceneResourceManager::instance()->get<Mesh>("box-mesh");

    translate(Vector3(swizzle<'x', '0', 'y'>(position)));
    setDuration(0.375f);

    connect(this, &Box::movedToTarget, this, &Box::animateMoveFromToTarget);
    connect(this, &Box::movedFromTarget, this, &Box::animateMoveFromToTarget);
}

void Box::draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>*) {
    shader->setTransformationMatrix(transformationMatrix)
          ->setDiffuseColor(color)
          ->use();

    mesh->draw();
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
