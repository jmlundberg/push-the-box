#ifndef PushTheBox_Game_WallBrick_h
#define PushTheBox_Game_WallBrick_h

/** @file
 * @brief Class PushTheBox::Game::WallBrick
 */

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief Wall brick */
class WallBrick: public Object3D, SceneGraph::Drawable<3> {
    public:
        /**
         * @brief Constructor
         * @param parent    Parent object
         * @param group     Drawable group
         */
        WallBrick(Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Mesh> mesh;
};

}}

#endif
