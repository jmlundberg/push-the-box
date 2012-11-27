#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "Box.h"
#include "Player.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level {
    friend class Player;

    public:
        enum class TileType {
            Empty = 0, Floor, Box, Wall, Target, BoxOnTarget
        };

        static Level* current();

        Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables);
        ~Level();

        inline Math::Vector2<int> size() const {
            return _size;
        }

        inline Math::Vector2<int> startingPosition() const {
            return _startingPosition;
        }

    private:
        void set(const Math::Vector2<int>& position, TileType type, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables);

        inline TileType& at(const Math::Vector2<int>& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        Box* boxAt(const Math::Vector2<int>& position);

        static Level* _current;

        Math::Vector2<int> _size;
        Math::Vector2<int> _startingPosition;
        std::vector<TileType> level;
        std::vector<Box*> boxes;
};

}}

#endif
