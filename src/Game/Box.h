#ifndef PushTheBox_Game_Box_h
#define PushTheBox_Game_Box_h

/** @file
 * @brief Class PushTheBox::Game::Box
 */

#include <Color.h>
#include <ResourceManager.h>
#include <SceneGraph/Drawable.h>
#include <SceneGraph/MatrixTransformation3D.h>
#include <Shaders/Shaders.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

/** @brief %Box */
class Box: public Object3D, public SceneGraph::Drawable<3> {
    friend class Level;
    friend class Player;

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
         * @param group     Drawable group
         */
        Box(const Vector2i& position, Type type, Object3D* parent = nullptr, SceneGraph::DrawableGroup<3>* group = nullptr);

        /** @seemagnum{SceneGraph::Drawable::draw()} */
        void draw(const Matrix4& transformationMatrix, SceneGraph::AbstractCamera<3>* camera) override;

    private:
        Resource<AbstractShaderProgram, Shaders::PhongShader> shader;
        Resource<Mesh> mesh;
        Vector2i position;
        Type type;
};

}}

#endif
