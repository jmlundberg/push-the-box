#ifndef PushTheBox_Game_Player_h
#define PushTheBox_Game_Player_h

/** @file
 * @brief Class PushTheBox::Game::Player
 */

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief %Player */
class Player: public Object3D, SceneGraph::Drawable<3> {
    public:
        /**
         * @brief Constructor
         * @param position  Initial position in level
         * @param parent    Parent object
         * @param group     Drawable group
         */
        Player(const Vector2i& position, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

        /** @brief Reset player to original position */
        void reset(const Vector2i& position);

        void move(const Vector2i& direction);

    private:
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Mesh> mesh, bodyMesh;
        Vector2i position;
};

}}

#endif
