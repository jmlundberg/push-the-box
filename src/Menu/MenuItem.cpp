#include "MenuItem.h"

#include <Physics/AxisAlignedBox.h>
#include <SceneGraph/AbstractCamera.h>
#include <SceneGraph/Drawable.h>
#include <Shaders/TextShader.h>
#include <Text/TextRenderer.h>
#include <Text/Font.h>

namespace PushTheBox { namespace Menu {

namespace {
    static const Color3<> off = Color3<>::fromHSV(Deg(210.0f), 0.55f, 0.9f);
    static const Color3<> on = Color3<>::fromHSV(Deg(210.0f), 0.85f, 0.9f);
}

MenuItem::MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup<2>* drawables, Physics::ObjectShapeGroup2D* shapes): Object2D(parent), SceneGraph::Drawable<2>(this, drawables), Physics::ObjectShape2D(this, shapes), color(off) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::TextShader2D>("text2d");
    font = SceneResourceManager::instance()->get<Text::Font>("font");

    /* Render text */
    Rectangle rect;
    std::tie(mesh, rect) = Text::TextRenderer2D::render(*font, 0.15f, title, &vertexBuffer, &indexBuffer, Buffer::Usage::StaticDraw);
    translate({-rect.width()/2, -0.075f});

    /* Shape for collision detection */
    setShape(Physics::AxisAlignedBox2D(rect.bottomLeft(), rect.topRight()));
}

void MenuItem::hoverChanged(bool hovered) {
    color = hovered ? on : off;
}

void MenuItem::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera<2>* camera) {
    shader->setColor(color)
        ->setTransformationProjectionMatrix(camera->projectionMatrix()*transformationMatrix)
        ->use();

    font->texture().bind(Shaders::TextShader2D::FontTextureLayer);

    mesh.draw();
}

}}
