#ifndef PushTheBox_Game_Hud_h
#define PushTheBox_Game_Hud_h

#include <Corrade/Interconnect/Receiver.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Animable.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/TranslationTransformation.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Text/Text.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class AbstractHudText: public Object2D, SceneGraph::Drawable2D, public Interconnect::Receiver {
    public:
        AbstractHudText(Object2D* parent, SceneGraph::DrawableGroup2D* drawables);

        ~AbstractHudText();

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::Camera2D& camera) override;

        Text::Renderer2D* text;
        Resource<Text::AbstractFont> font;
        Resource<Text::GlyphCache> glyphCache;

    private:
        Resource<AbstractShaderProgram, Magnum::Shaders::DistanceFieldVector2D> shader;
};

class LevelTitle: public AbstractHudText {
    public:
        LevelTitle(Object2D* parent, SceneGraph::DrawableGroup2D* drawables);

        void update(const std::string& name);
};

class RemainingTargets: public AbstractHudText, SceneGraph::Animable2D {
    public:
        RemainingTargets(Object2D* parent, SceneGraph::DrawableGroup2D* drawables, SceneGraph::AnimableGroup2D* animables);

        void update(UnsignedInt count);

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::Camera2D& camera) override;
        void animationStep(Float time, Float delta) override;
        void animationStopped() override;

    private:
        Float scale;
};

class Moves: public AbstractHudText {
    public:
        Moves(Object2D* parent, SceneGraph::DrawableGroup2D* drawables);

        void update(UnsignedInt count);
};

}}

#endif
