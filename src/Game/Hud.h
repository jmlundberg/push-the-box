#ifndef PushTheBox_Game_Hud_h
#define PushTheBox_Game_Hud_h

#include <Interconnect/Receiver.h>
#include <ResourceManager.h>
#include <SceneGraph/Animable.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/DualComplexTransformation.h>
#include <Shaders/DistanceFieldVector.h>
#include <Text/Text.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class AbstractHudText: public Object2D, SceneGraph::Drawable2D, public Interconnect::Receiver {
    public:
        AbstractHudText(Object2D* parent, SceneGraph::DrawableGroup2D* drawables);

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D* camera) override;

        Text::TextRenderer2D* text;

    private:
        Resource<AbstractShaderProgram, Magnum::Shaders::DistanceFieldVector2D> shader;
        Resource<Text::AbstractFont> font;
        Resource<Text::GlyphCache> glyphCache;
};

class RemainingTargets: public AbstractHudText, SceneGraph::Animable2D {
    public:
        RemainingTargets(Object2D* parent, SceneGraph::DrawableGroup2D* drawables, SceneGraph::AnimableGroup2D* animables);

        void update(UnsignedInt count);

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera2D* camera) override;
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
