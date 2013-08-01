#include "Game/Hud.h"

#include <SceneGraph/AbstractCamera.h>
#include <Text/AbstractFont.h>
#include <Text/GlyphCache.h>
#include <Text/TextRenderer.h>

#ifdef CORRADE_TARGET_NACL_NEWLIB
#include <sstream>
#endif

namespace PushTheBox { namespace Game {

AbstractHudText::AbstractHudText(Object2D* parent, SceneGraph::DrawableGroup2D* drawables): Object2D(parent), SceneGraph::Drawable2D(*this, drawables), shader(SceneResourceManager::instance().get<AbstractShaderProgram, Shaders::DistanceFieldVector2D>("text2d")), font(SceneResourceManager::instance().get<Text::AbstractFont>("font")), glyphCache(SceneResourceManager::instance().get<Text::GlyphCache>("cache")) {
    text = new Text::TextRenderer2D(*font, *glyphCache, 0.06f);
}

void AbstractHudText::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D& camera) {
    shader->setTransformationProjectionMatrix(camera.projectionMatrix()*transformationMatrix)
        .setColor(Color3(1.0f))
        .setOutlineRange(0.5f, 1.0f)
        .use();

    glyphCache->texture().bind(Shaders::DistanceFieldVector2D::VectorTextureLayer);

    text->mesh().draw();
}

LevelTitle::LevelTitle(Object2D* parent, SceneGraph::DrawableGroup2D* drawables): AbstractHudText(parent, drawables) {
    text->reserve(32, Buffer::Usage::StaticDraw, Buffer::Usage::StaticDraw);
}

void LevelTitle::update(const std::string& name) {
    text->render(name);

    resetTransformation();
    translate({-1.303f, 0.97f - text->rectangle().top()});
}

RemainingTargets::RemainingTargets(Object2D* parent, SceneGraph::DrawableGroup2D* drawables, SceneGraph::AnimableGroup2D* animables): AbstractHudText(parent, drawables), SceneGraph::Animable2D(*this, animables) {
    text->reserve(32, Buffer::Usage::DynamicDraw, Buffer::Usage::StaticDraw);

    setDuration(0.4f);
    setRepeated(true);
    setRepeatCount(2);

    translate({-1.303f, -0.97f});
}

void RemainingTargets::update(UnsignedInt count) {
    #ifndef CORRADE_TARGET_NACL_NEWLIB
    text->render(std::to_string(count) + " remaining targets");
    #else
    std::ostringstream out;
    out << count << " remaining targets";
    text->render(out.str());
    #endif

    setState(SceneGraph::AnimationState::Running);
}

void RemainingTargets::draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D& camera) {
    AbstractHudText::draw(transformationMatrix*Matrix3::scaling(Vector2(scale)), camera);
}

void RemainingTargets::animationStep(Float time, Float) {
    scale = 1.0f + Math::sin(Rad(Constants::pi()*time/duration()))*0.35f;
}

void RemainingTargets::animationStopped() {
    scale = 1.0f;
}

Moves::Moves(Object2D* parent, SceneGraph::DrawableGroup2D* drawables): AbstractHudText(parent, drawables) {
    text->reserve(32, Buffer::Usage::DynamicDraw, Buffer::Usage::StaticDraw);
}

void Moves::update(UnsignedInt count) {
    #ifndef CORRADE_TARGET_NACL_NEWLIB
    text->render(std::to_string(count) + " moves");
    #else
    std::ostringstream out;
    out << count << " moves";
    text->render(out.str());
    #endif

    resetTransformation();
    translate({1.303f - text->rectangle().width(), 0.97f - text->rectangle().top()});
}

}}
