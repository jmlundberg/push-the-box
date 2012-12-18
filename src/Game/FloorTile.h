#ifndef PushTheBox_Game_FloorTile_h
#define PushTheBox_Game_FloorTile_h

/** @file
 * @brief Class PushTheBox::Game::FloorTile
 */

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/EuclideanMatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/**
@brief Floor tile

Tile at -Y side of unit cube.
*/
class FloorTile: public Object3D, SceneGraph::Drawable<3> {
    public:
        enum class Type {
            Floor, Target
        };

        /**
         * @brief Constructor
         * @param position  Position in level
         * @param type      Tile type
         * @param parent    Parent object
         * @param group     Drawable group
         */
        FloorTile(const Vector2i& position, Type type, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Mesh> mesh;
        const Type type;
};

}}

#endif
