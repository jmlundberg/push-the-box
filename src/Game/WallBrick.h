#ifndef PushTheBox_Game_WallBrick_h
#define PushTheBox_Game_WallBrick_h

/** @file
 * @brief Class PushTheBox::Game::WallBrick
 */

#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief Wall brick */
class WallBrick: public Object3D, SceneGraph::Drawable3D {
    public:
        /**
         * @brief Constructor
         * @param position  Position in level
         * @param parent    Parent object
         * @param group     Drawable group
         */
        WallBrick(const Vector2i& position, Object3D* parent = nullptr, SceneGraph::DrawableGroup3D* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::Phong> shader;
        Resource<Mesh> mesh;
};

}}

#endif
