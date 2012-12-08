#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "Box.h"
#include "Player.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level: public Object3D {
    friend class Player;

    public:
        enum class TileType {
            Empty = 0, Floor, Box, Wall, Target, BoxOnTarget
        };

        static Level* current();

        Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables);
        ~Level();

        inline std::string name() const { return _name; }

        inline std::string nextLevel() const { return _nextName; }

        inline Vector2i size() const { return _size; }

        inline Vector2i startingPosition() const { return _startingPosition; }

        std::size_t targetsRemain;

    private:
        void set(const Vector2i& position, TileType type, SceneGraph::DrawableGroup<3>* drawables);

        inline TileType& at(const Vector2i& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        Box* boxAt(const Vector2i& position);

        static Level* _current;

        std::string _name, _nextName;
        Vector2i _size, _startingPosition;
        std::vector<TileType> level;
        std::vector<Box*> boxes;
};

}}

#endif
