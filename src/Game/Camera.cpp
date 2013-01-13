#include "Camera.h"

#include <Math/Constants.h>

namespace PushTheBox { namespace Game {

Camera::Camera(Object3D* parent): Object3D(parent), SceneGraph::Camera3D<>(this) {
    setPerspective(deg(35.0f), 1.0f, 0.001f, 100.0f);
    setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);
}

}}
