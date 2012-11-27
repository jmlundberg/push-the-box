#include "Level.h"

#include <Math/Vector2.h>

using namespace std;

namespace PushTheBox { namespace Game {

Level::Level(const std::string&) {
    //load levelu ze souboru podle jmena...zatim ukazkovy level na vyzkouseni...
    for(size_t i = 0; i != 5; ++i) {
        vector<FieldType> line;
        for(std::size_t j = 0; j < 5; ++j) {
            line.push_back(FieldType::Empty);
        }
        level.push_back(line);
    }

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
    level[from.x()][from.y()] = FieldType::Empty;
    level[to.x()][to.y()] = FieldType::Box;

    box(from)->move(to-from);
}

void Level::set(const Math::Vector2<int>& position, FieldType type) {
    level[position.x()][position.y()] = type;

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
