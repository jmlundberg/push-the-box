#ifndef PushTheBox_MenuItem_h
#define PushTheBox_MenuItem_h

/** @file
 * @brief Class PushTheBox::Menu::MenuItem
 */

#include <Physics/Physics.h>
#include <SceneGraph/EuclideanMatrixTransformation2D.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

/** @brief %Menu item */
class MenuItem: public Object2D {
    public:
        /**
         * @brief Constructor
         * @param parent            Parent object
         * @param drawableGroup     Drawable group
         * @param shapeGroup        Shape group
         */
        MenuItem(Object2D* parent, SceneGraph::DrawableGroup<2>* drawableGroup, Physics::ObjectShapeGroup2D* shapeGroup);
};

}}

#endif
