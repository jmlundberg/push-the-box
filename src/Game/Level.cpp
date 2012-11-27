#include "Level.h"

#include <Math/Vector2.h>

using namespace std;

namespace PushTheBox { namespace Game {

Level::Level(const std::string&) {
    _size = {5, 5};

    level.resize(_size.product(), TileType::Floor);

    set({0, 0}, TileType::Target);
    set({0, 4}, TileType::Target);
    set({4, 0}, TileType::Target);
    set({4, 4}, TileType::Target);

    set({2, 1}, TileType::Box);
    set({1, 2}, TileType::Box);
    set({2, 3}, TileType::Box);
    set({3, 2}, TileType::Box);
}

void Level::moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to) {
    at(from) = TileType::Floor;
    at(to) = TileType::Box;

    box(from)->move(to-from);
}

void Level::set(const Math::Vector2<int>& position, TileType type) {
    at(position) = type;

    if(type == TileType::Box) boxes.push_back(new Box(position));
}

Box* Level::box(const Math::Vector2<int>& position) {
    for(size_t i = 0; i < boxes.size(); ++i) {
        if(boxes[i]->position() == position) {
            return boxes[i];
        }
    }
    return nullptr;
}

}}
