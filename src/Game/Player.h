#ifndef PushTheBox_Game_Player_h
#define PushTheBox_Game_Player_h

/** @file
 * @brief Class PushTheBox::Game::Player
 */

#include <Magnum/ResourceManager.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/SceneGraph/DualQuaternionTransformation.h>
#include <Magnum/Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief %Player */
class Player: public Object3D, SceneGraph::Drawable3D {
    public:
        /**
         * @brief Constructor
         * @param parent        Parent object
         * @param drawables     Drawable group
         */
        Player(Object3D* parent, SceneGraph::DrawableGroup3D* drawables);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

    private:
        Resource<AbstractShaderProgram, Magnum::Shaders::Phong> shader;
        Resource<Mesh> mesh, bodyMesh;
};

}}

#endif
