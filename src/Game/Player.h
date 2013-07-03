#ifndef PushTheBox_Game_Player_h
#define PushTheBox_Game_Player_h

/** @file
 * @brief Class PushTheBox::Game::Player
 */

#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/DualQuaternionTransformation.h>
#include <Shaders/Shaders.h>

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
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera3D* camera) override;

    private:
        Resource<AbstractShaderProgram, Magnum::Shaders::Phong> shader;
        Resource<Mesh> mesh, bodyMesh;
};

}}

#endif
