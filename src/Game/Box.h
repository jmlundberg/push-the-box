#ifndef PushTheBox_Game_Box_h
#define PushTheBox_Game_Box_h

/** @file
 * @brief Class PushTheBox::Game::Box
 */

#include <Interconnect/Emitter.h>
#include <Interconnect/Receiver.h>
#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Animable.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/DualQuaternionTransformation.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief %Box */
class Box: public Object3D, public SceneGraph::Drawable<3>, public SceneGraph::Animable<3>, public Interconnect::Emitter, public Interconnect::Receiver {
    friend class Level;

    public:
        enum class Type {
            OnFloor,
            OnTarget
        };

        /**
         * @brief Constructor
         * @param position  Initial position in level
         * @param type      Box type
         * @param parent    Parent object
         * @param drawables Drawable group
         * @param animables Animable group
         */
        Box(const Vector2i& position, Type type, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* drawables = nullptr, SceneGraph::AnimableGroup<3>* animables = nullptr);

        /** @brief Box was moved to target */
        inline Signal movedToTarget() {
            return emit(&Box::movedToTarget);
        }

        /** @brief Box was moved from target */
        inline Signal movedFromTarget() {
            return emit(&Box::movedFromTarget);
        }

    protected:
        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

        /** @seemagnum{SceneGraph::Animable::animationStep()} */
        void animationStep(Float time, Float delta) override;

        /** @seemagnum{SceneGraph::Animable::animationStopped()} */
        void animationStopped() override;

    private:
        inline void animateMoveFromToTarget() {
            setState(SceneGraph::AnimationState::Running);
        }

        Resource<AbstractShaderProgram, Shaders::Phong> shader;
        Resource<Mesh> mesh;
        Vector2i position;
        Type type;
        Color3<> color;
};

}}

#endif
