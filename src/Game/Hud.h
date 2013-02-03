#ifndef PushTheBox_Game_Hud_h
#define PushTheBox_Game_Hud_h

#include <Interconnect/Receiver.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/EuclideanMatrixTransformation2D.h>
#include <Shaders/TextShader.h>
#include <Text/Text.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class AbstractHudText: public Object2D, SceneGraph::Drawable2D<>, public Corrade::Interconnect::Receiver {
    public:
        AbstractHudText(Object2D* parent, SceneGraph::DrawableGroup2D<>* drawables);

    protected:
        void draw(const Matrix3& transformationMatrix, SceneGraph::AbstractCamera<2>* camera) override;

        Text::TextRenderer2D* text;

    private:
        Resource<AbstractShaderProgram, Magnum::Shaders::TextShader2D> shader;
        Resource<Text::Font> font;
};

class RemainingTargets: public AbstractHudText {
    public:
        RemainingTargets(Object2D* parent, SceneGraph::DrawableGroup2D<>* drawables);

        void update(std::uint32_t count);
};

class Moves: public AbstractHudText {
    public:
        Moves(Object2D* parent, SceneGraph::DrawableGroup2D<>* drawables);

        void update(std::uint32_t count);
};

}}

#endif
