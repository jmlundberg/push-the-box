#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "Box.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level {
    public:
        enum class TileType {
            Empty = 0, Floor, Box, Wall, Target, BoxOnTarget
        };

        static Level* instance();

        Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables);

        inline Math::Vector2<int> size() const {
            return _size;
        }

        inline TileType value(const Math::Vector2<int>& position) const {
            return level[position.y()*_size.x()+position.x()];
        }

        Box* box(const Math::Vector2<int>& position);

        void moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to);

    protected:
        void set(const Math::Vector2<int>& position, TileType type, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables);

    private:
        inline TileType& at(const Math::Vector2<int>& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        Math::Vector2<int> _size;
        std::vector<TileType> level;
        std::vector<Box*> boxes;
};

}}

#endif
