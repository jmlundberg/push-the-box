#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include <vector>
#include <string>
#include <Interconnect/Emitter.h>
#include <SceneGraph/DualQuaternionTransformation.h>

#include "PushTheBox.h"

namespace PushTheBox { namespace Game {

class Box;

/** @brief %Level */
class Level: public Object3D, public Interconnect::Emitter {
    public:
        enum class TileType {
            Empty = 0, Floor, Box, Wall, Target, BoxOnTarget
        };

        /**
         * @brief Constructor
         * @param name          Level name
         * @param scene         Scene to which to add the level
         * @param drawables     Drawable group
         * @param animables     Animable group
         */
        Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup3D* drawables, SceneGraph::AnimableGroup3D* animables);

        /** @brief Level name */
        inline std::string name() const { return _name; }

        /** @brief Next level name */
        inline std::string nextName() const { return _nextName; }

        /** @brief Level size */
        inline Vector2i size() const { return _size; }

        /** @brief Player position */
        inline Vector2i playerPosition() { return _playerPosition; }

        /** @brief Remaining targets */
        inline UnsignedInt remainingTargets() const { return _remainingTargets; }

        /** @brief Remaining targets changed */
        inline Signal remainingTargetsChanged(UnsignedInt count) {
            return emit<Level, UnsignedInt>(&Level::remainingTargetsChanged, count);
        }

        /** @brief Player moves */
        inline UnsignedInt moves() const { return _moves; }

        /** @brief Player moves changed */
        inline Signal movesChanged(UnsignedInt count) {
            return emit<Level, UnsignedInt>(&Level::movesChanged, count);
        }

        /**
         * @brief Move player in given direction
         * @return `True` if the player moved, `false` otherwise
         */
        bool movePlayer(const Vector2i& direction);

    private:
        void set(const Vector2i& position, TileType type, SceneGraph::DrawableGroup3D* drawables, SceneGraph::AnimableGroup3D* animables);

        inline TileType& at(const Vector2i& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        std::string _name, _nextName;
        Vector2i _size, _playerPosition;
        UnsignedInt _remainingTargets, _moves;
        std::vector<TileType> level;
        std::vector<Box*> boxes;
};

}}

#endif
