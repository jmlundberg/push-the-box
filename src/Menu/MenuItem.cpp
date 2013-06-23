#include "MenuItem.h"

#include <ResourceManager.h>
#include <SceneGraph/AbstractCamera.h>
#include <SceneGraph/Drawable.h>
#include <Shaders/DistanceFieldVector.h>
#include <Shapes/AxisAlignedBox.h>
#include <Text/AbstractFont.h>
#include <Text/GlyphCache.h>
#include <Text/TextRenderer.h>

namespace PushTheBox { namespace Menu {

namespace {
    const Color3<> outline = Color3<>(1.0f);
    const Color3<> off = Color3<>::fromHSV(Deg(210.0f), 0.55f, 0.9f);
    const Color3<> on = Color3<>::fromHSV(Deg(210.0f), 0.85f, 0.9f);
}

MenuItem::MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup<2>* drawables, Shapes::ShapeGroup2D* shapes): Object2D(parent), SceneGraph::Drawable<2>(this, drawables), Shapes::Shape<Shapes::AxisAlignedBox2D>(this, shapes), color(off) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::DistanceFieldVector2D>("text2d");
    auto font = SceneResourceManager::instance()->get<Text::AbstractFont>("font");
    glyphCache = SceneResourceManager::instance()->get<Text::GlyphCache>("cache");

    /* Render text */
    Magnum::Rectangle rect; /* Rectangle conflicts with WINAPI, damn */
    std::tie(mesh, rect) = Text::TextRenderer2D::render(font, glyphCache, 0.15f, title, &vertexBuffer, &indexBuffer, Buffer::Usage::StaticDraw);
    translate({-rect.width()/2, -0.075f});

    /* Shape for collision detection */
    setShape({rect.bottomLeft(), rect.topRight()});
}

void MenuItem::hoverChanged(bool hovered) {
    color = hovered ? on : off;
}

void MenuItem::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera<2>* camera) {
    shader->setColor(color)
        ->setOutlineColor(outline)
        ->setOutlineRange(0.55f, 0.45f)
        ->setTransformationProjectionMatrix(camera->projectionMatrix()*transformationMatrix)
        ->use();

    glyphCache->texture()->bind(Shaders::DistanceFieldVector2D::VectorTextureLayer);

    mesh.draw();
}

}}
