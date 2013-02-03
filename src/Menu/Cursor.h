#ifndef PushTheBox_Menu_Cursor_h
#define PushTheBox_Menu_Cursor_h

#include <Physics/ObjectShapeGroup.h>
#include <SceneGraph/EuclideanMatrixTransformation2D.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

class Cursor: public Object2D, public Physics::ObjectShape2D {
    public:
        explicit Cursor(Object2D* parent, Physics::ObjectShapeGroup2D* shapes);
};

}}

#endif
