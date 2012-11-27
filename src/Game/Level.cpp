#include "Level.h"

#include <Math/Vector2.h>

using namespace std;

namespace PushTheBox { namespace Game {

Level::Level(const std::string&) {
    _size = {5, 5};

    level.resize(_size.product(), FieldType::Empty);

    set({0, 0}, FieldType::Target);
    set({0, 4}, FieldType::Target);
    set({4, 0}, FieldType::Target);
    set({4, 4}, FieldType::Target);

    set({2, 1}, FieldType::Box);
    set({1, 2}, FieldType::Box);
    set({2, 3}, FieldType::Box);
    set({3, 2}, FieldType::Box);
}

void Level::moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to) {
    at(from) = FieldType::Empty;
    at(to) = FieldType::Box;

    box(from)->move(to-from);
}

void Level::set(const Math::Vector2<int>& position, FieldType type) {
    at(position) = type;

    if(type == FieldType::Box) boxes.push_back(new Box(position));
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
