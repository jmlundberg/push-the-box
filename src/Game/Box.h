#ifndef PushTheBox_Game_Box_h
#define PushTheBox_Game_Box_h

/** @file
 * @brief Class PushTheBox::Game::Box
 */

#include <Corrade/Interconnect/Emitter.h>
#include <Corrade/Interconnect/Receiver.h>
#include <Magnum/Color.h>
#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Animable.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief %Box */
class Box: public Object3D, public SceneGraph::Drawable3D, public SceneGraph::Animable3D, public Interconnect::Emitter, public Interconnect::Receiver {
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
        Box(const Vector2i& position, Type type, Object3D* parent = nullptr, SceneGraph::DrawableGroup3D* drawables = nullptr, SceneGraph::AnimableGroup3D* animables = nullptr);

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
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera3D& camera) override;

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
        Color3 color;
};

}}

#endif
