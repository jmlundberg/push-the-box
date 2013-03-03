#ifndef PushTheBox_Menu_MenuItem_h
#define PushTheBox_Menu_MenuItem_h

/** @file
 * @brief Class PushTheBox::Menu::MenuItem
 */

#include <Buffer.h>
#include <Color.h>
#include <Mesh.h>
#include <Resource.h>
#include <Physics/ObjectShape.h>
#include <Physics/Physics.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/EuclideanMatrixTransformation2D.h>
#include <Shaders/Shaders.h>
#include <Interconnect/Emitter.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Menu {

/** @brief %Menu item */
class MenuItem: public Object2D, SceneGraph::Drawable<2>, public Physics::ObjectShape2D, public Corrade::Interconnect::Emitter {
    public:
        /**
         * @brief Constructor
         * @param title         Title
         * @param parent        Parent object
         * @param drawables     Drawable group
         * @param shapes        Shape group
         */
        MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup<2>* drawableGroup, Physics::ObjectShapeGroup2D* shapeGroup);

        void hoverChanged(bool hovered);

        inline Signal clicked() {
            return emit(&MenuItem::clicked);
        }

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera<2>* camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::DistanceFieldVectorShader2D> shader;
        Resource<Text::Font> font;
        Buffer vertexBuffer, indexBuffer;
        Mesh mesh;
        Color3<> color, outlineColor;
};

}}

#endif
