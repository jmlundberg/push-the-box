#include "MenuItem.h"

#include <Physics/AxisAlignedBox.h>
#include <DebugTools/ShapeRenderer.h>
#include <Physics/ObjectShape.h>
#include <SceneGraph/AbstractCamera.h>
#include <SceneGraph/Drawable.h>
#include <Text/TextRenderer.h>
#include <Text/Font.h>
#include <Shaders/TextShader.h>

namespace PushTheBox { namespace Menu {

MenuItem::MenuItem(const std::string& title, Object2D* parent, SceneGraph::DrawableGroup<2>* drawableGroup, Physics::ObjectShapeGroup2D* shapeGroup): Object2D(parent), SceneGraph::Drawable<2>(this, drawableGroup) {
    shader = SceneResourceManager::instance()->get<AbstractShaderProgram, Shaders::TextShader2D>("text2d");
    font = SceneResourceManager::instance()->get<Text::Font>("font");

    /* Render text */
    Rectangle rect;
    std::tie(mesh, rect) = Text::TextRenderer2D::render(*font, 0.15f, title, &vertexBuffer, &indexBuffer, Buffer::Usage::StaticDraw);
    translate({-rect.width()/2, -0.075f});

    /* Shape for collision detection */
    Physics::ObjectShape2D* shape = new Physics::ObjectShape2D(this, shapeGroup);
    shape->setShape(Physics::AxisAlignedBox2D({rect.width()/2, rect.height()/2 - rect.bottom()}, rect.size()));

    /** @todo remove when debugging no more */
    new DebugTools::ShapeRenderer2D(shape, "menu", drawableGroup);
}

void MenuItem::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera<2>* camera) {
    shader->setColor(Color3<>::fromHSV(210.0f, 0.55f, 0.9f))
        ->setTransformationProjectionMatrix(camera->projectionMatrix()*transformationMatrix)
        ->use();

    font->texture().bind(Shaders::TextShader2D::FontTextureLayer);

    mesh.draw();
}

}}
