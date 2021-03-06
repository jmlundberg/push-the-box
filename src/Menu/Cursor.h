#ifndef PushTheBox_Menu_Cursor_h
#define PushTheBox_Menu_Cursor_h

#include <Magnum/SceneGraph/TranslationTransformation.h>
#include <Magnum/Shapes/Point.h>
#include <Magnum/Shapes/Shape.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

class Cursor: public Object2D, public Shapes::Shape<Shapes::Point2D> {
    public:
        explicit Cursor(Object2D* parent, Shapes::ShapeGroup2D* shapes);
};

}}

#endif
