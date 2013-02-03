#include "Cursor.h"

#include <Physics/Point.h>

namespace PushTheBox { namespace Menu {

Cursor::Cursor(Object2D* parent, Physics::ObjectShapeGroup2D* shapes): Object2D(parent), Physics::ObjectShape2D(this, shapes) {
    setShape(Physics::Point2D({}));
}

}}
