#ifndef PushTheBox_Game_WallTile_h
#define PushTheBox_Game_WallTile_h

/** @file
 * @brief Class PushTheBox::Game::WallTile
 */

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/**
@brief Wall tile

Tile on +Z side of unit cube.
*/
class WallTile: public Object3D, SceneGraph::Drawable<3> {
    public:
        /**
         * @brief Constructor
         * @param color     Color of this tile
         * @param parent    Parent object
         * @param group     Drawable group
         */
        WallTile(const Color3<GLfloat>& color, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

    private:
        Color3<GLfloat> color;
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Buffer> buffer;
        Resource<Mesh> mesh;
};

}}

#endif
