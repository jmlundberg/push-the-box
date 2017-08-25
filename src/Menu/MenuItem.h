#ifndef PushTheBox_Menu_MenuItem_h
#define PushTheBox_Menu_MenuItem_h

/** @file
 * @brief Class PushTheBox::Menu::MenuItem
 */

#include <Corrade/Interconnect/Emitter.h>
#include <Magnum/Buffer.h>
#include <Magnum/Color.h>
#include <Magnum/Mesh.h>
#include <Magnum/Resource.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>
#include <Magnum/Shaders/Shaders.h>
#include <Magnum/Shapes/AxisAlignedBox.h>
#include <Magnum/Shapes/Shape.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

/** @brief %Menu item */
class MenuItem: public Object2D, SceneGraph::Drawable2D, public Shapes::Shape<Shapes::AxisAlignedBox2D>, public Interconnect::Emitter {
    public:
        /**
         * @brief Constructor
         * @param title         Title
         * @param parent        Parent object
         * @param drawables     Drawable group
         * @param shapes        Shape group
         */
        MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup2D* drawableGroup, Shapes::ShapeGroup2D* shapes);

        void hoverChanged(bool hovered);

        inline Signal clicked() {
            return emit(&MenuItem::clicked);
        }

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::Camera2D& camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::DistanceFieldVector2D> shader;
        Resource<Text::GlyphCache> glyphCache;
        Buffer vertexBuffer, indexBuffer;
        Mesh mesh;
        Color3 color;
};

}}

#endif
