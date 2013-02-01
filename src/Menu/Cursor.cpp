#include "Cursor.h"

#include <DebugTools/ShapeRenderer.h>
#include <Physics/ObjectShape.h>
#include <Physics/Point.h>

namespace PushTheBox { namespace Menu {

Cursor::Cursor(Object2D* parent, SceneGraph::DrawableGroup2D<>* drawableGroup, Physics::ObjectShapeGroup2D* objectShapeGroup): Object2D(parent), Physics::ObjectShape2D(this, objectShapeGroup) {
    setShape(Physics::Point2D({}));

    /** @todo remove when debugging no more */
    new DebugTools::ShapeRenderer2D(this, "menu", drawableGroup);
}

}}
