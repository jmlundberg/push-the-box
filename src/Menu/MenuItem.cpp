#include "MenuItem.h"

#include <Physics/AxisAlignedBox.h>
#include <DebugTools/ShapeRenderer.h>
#include <Physics/ObjectShape.h>
#include <SceneGraph/Drawable.h>

namespace PushTheBox { namespace Menu {

MenuItem::MenuItem(Object2D* parent, SceneGraph::DrawableGroup<2>* drawableGroup, Physics::ObjectShapeGroup2D* shapeGroup): Object2D(parent) {
    /* Shape for collision detection */
    Physics::ObjectShape2D* shape = new Physics::ObjectShape2D(this, shapeGroup);
    shape->setShape(new Physics::AxisAlignedBox2D({}, {0.6f, 0.1f}));

    /* Draw some shape on the screen actually */
    /** @todo Do this seriously with text and shit */
    new DebugTools::ShapeRenderer2D(shape, "menu", drawableGroup);
}

}}
