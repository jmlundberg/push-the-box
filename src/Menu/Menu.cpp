#include "Menu.h"

#include <Utility/Resource.h>
#include <DefaultFramebuffer.h>
#include <Renderer.h>
#include <DebugTools/ShapeRenderer.h>
#include <SceneGraph/Camera2D.h>
#include <Shaders/TextShader.h>
#include <Text/Font.h>

#include "Cursor.h"
#include "MenuItem.h"

namespace PushTheBox { namespace Menu {

Menu::Menu(Text::FontRenderer& fontRenderer) {
    /* Configure camera */
    (cameraObject = new Object2D(&scene));
    (camera = new SceneGraph::Camera2D<>(cameraObject))
        ->setProjection({8.0f/3.0f, 2.0f})
        ->setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend);

    /* Text rendering... */
    SceneResourceManager::instance()->set<AbstractShaderProgram>("text2d", new Shaders::TextShader2D);
    Corrade::Utility::Resource rs("PushTheBoxData");
    const unsigned char* fontData;
    std::size_t fontSize;
    std::tie(fontData, fontSize) = rs.getRaw("luckiest-guy.ttf");
    Text::Font* font = new Text::Font(fontRenderer, fontData, fontSize, 96.0f);
    font->prerender("abcdefghijklmnopqrstuvwxyz0123456789 ", Vector2i(768));
    SceneResourceManager::instance()->set("font", font);

    /* Configure color of menu debug shapes */
    /** @todo remove when done properly */
    debugDrawResourceManager.set("menu", (new DebugTools::ShapeRendererOptions)
        ->setColor(Color3<>::fromHSV(240.0f, 0.2f, 0.5f))->setPointSize(0.10f));

    /* Add menu items */
    (new MenuItem("resume", &scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(0.45f));
    (new MenuItem("restart level", &scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(0.15f));
    (new MenuItem("new game", &scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(-0.15f));
    (new MenuItem("credits", &scene, &drawables, &shapes))
        ->translate(Vector2::yAxis(-0.45f));

    /* Add cursor */
    (cursor = new Cursor(&scene, &drawables, &shapes))
        ->translate({-10.0f, -10.0f});
}

void Menu::focusEvent() {
    setPropagatedEvents(PropagatedEvent::Draw|PropagatedEvent::Input);
}

void Menu::blurEvent() {
    setPropagatedEvents({});
}

void Menu::viewportEvent(const Vector2i& size) {
    camera->setViewport(size);
}

void Menu::drawEvent() {
    Renderer::setFeature(Renderer::Feature::Blending, true);
    Renderer::setBlendFunction(Renderer::BlendFunction::One, Renderer::BlendFunction::OneMinusSourceAlpha);
    Renderer::setFeature(Renderer::Feature::DepthTest, false);
    /** @todo fix this in magnum, so it doesn't have to be called? */
    shapes.setClean();
    camera->draw(drawables);
    Renderer::setFeature(Renderer::Feature::DepthTest, true);
    Renderer::setFeature(Renderer::Feature::Blending, false);
}

void Menu::mousePressEvent(MouseEvent& event) {
    if(event.button() == MouseEvent::Button::Left)
        application()->focusScreen(application()->backScreen()); /** @todo Implement me better */
    else return;

    event.setAccepted();
    redraw();
}

void Menu::mouseMoveEvent(MouseMoveEvent& event) {
    Vector2 cursorPosition =
        Vector2::yScale(-1.0f)*(Vector2(event.position())/defaultFramebuffer.viewport().size()-Vector2(0.5f))*camera->projectionSize();

    MenuItem* collisionBefore = static_cast<MenuItem*>(shapes.firstCollision(cursor));
    cursor->resetTransformation()->translate(cursorPosition);
    MenuItem* collisionAfter = static_cast<MenuItem*>(shapes.firstCollision(cursor));

    if(collisionBefore != collisionAfter) {
        if(collisionBefore) collisionBefore->hoverChanged(false);
        if(collisionAfter) collisionAfter->hoverChanged(true);
    }

    event.setAccepted();
    redraw();
}

}}
