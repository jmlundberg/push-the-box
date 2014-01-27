#include "MenuItem.h"

#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/AbstractCamera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Shapes/AxisAlignedBox.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/GlyphCache.h>
#include <Magnum/Text/Renderer.h>

namespace PushTheBox { namespace Menu {

namespace {
    const Color3 outline = Color3(1.0f);
    const Color3 off = Color3::fromHSV(Deg(210.0f), 0.55f, 0.9f);
    const Color3 on = Color3::fromHSV(Deg(210.0f), 0.85f, 0.9f);
}

MenuItem::MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup2D* drawables, Shapes::ShapeGroup2D* shapes): Object2D(parent), SceneGraph::Drawable2D(*this, drawables), Shapes::Shape<Shapes::AxisAlignedBox2D>(*this, shapes), vertexBuffer(Buffer::Target::Array), indexBuffer(Buffer::Target::ElementArray), color(off) {
    shader = SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::DistanceFieldVector2D>("text2d");
    auto font = SceneResourceManager::instance().get<Text::AbstractFont>("font");
    glyphCache = SceneResourceManager::instance().get<Text::GlyphCache>("cache");

    /* Render text */
    Range2D rect;
    std::tie(mesh, rect) = Text::Renderer2D::render(font, glyphCache, 0.15f, title, vertexBuffer, indexBuffer, BufferUsage::StaticDraw, Text::Alignment::MiddleCenter);

    /* Shape for collision detection */
    setShape({rect.bottomLeft(), rect.topRight()});
}

void MenuItem::hoverChanged(bool hovered) {
    color = hovered ? on : off;
}

void MenuItem::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D& camera) {
    shader->setColor(color)
        .setOutlineColor(outline)
        .setOutlineRange(0.55f, 0.45f)
        .setTransformationProjectionMatrix(camera.projectionMatrix()*transformationMatrix)
        .use();

    glyphCache->texture().bind(Shaders::DistanceFieldVector2D::VectorTextureLayer);

    mesh.draw();
}

}}
