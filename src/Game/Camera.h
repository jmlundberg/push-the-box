#ifndef PushTheBox_Game_Camera_h
#define PushTheBox_Game_Camera_h

#include <SceneGraph/Camera3D.h>
#include <SceneGraph/EuclideanMatrixTransformation3D.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class Camera: public Object3D, public SceneGraph::Camera3D<> {
    public:
        Camera(Object3D* parent = nullptr);
};

}}

#endif
