#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "Box.h"
#include "Player.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level: public Object3D {
    public:
        enum class TileType {
            Empty = 0, Floor, Box, Wall, Target, BoxOnTarget
        };

        static Level* current();

        Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables, SceneGraph::AnimableGroup<3>* animables);
        ~Level();

        inline std::string name() const { return _name; }

        inline std::string nextLevel() const { return _nextName; }

        inline Vector2i size() const { return _size; }

        /**
         * @brief Reset player to starting position in this level
         * @return Pointer to self (for method chaining)
         */
        Level* resetPlayer(Player* player);

        /** @brief Move player in given direction */
        void movePlayer(Player* player, const Vector2i& direction);

        /** @brief Remaining targets */
        inline std::size_t remainingTargets() const { return _remainingTargets; }

    private:
        void set(const Vector2i& position, TileType type, SceneGraph::DrawableGroup<3>* drawables, SceneGraph::AnimableGroup<3>* animables);

        inline TileType& at(const Vector2i& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        Box* boxAt(const Vector2i& position);

        static Level* _current;

        std::string _name, _nextName;
        Vector2i _size, playerPosition;
        std::vector<TileType> level;
        std::vector<Box*> boxes;
        std::size_t _remainingTargets;
};

}}

#endif
