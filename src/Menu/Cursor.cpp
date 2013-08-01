#include "Cursor.h"

#include <Shapes/Point.h>

namespace PushTheBox { namespace Menu {

Cursor::Cursor(Object2D* parent, Shapes::ShapeGroup2D* shapes): Object2D(parent), Shapes::Shape<Shapes::Point2D>(*this, {}, shapes) {}

}}
