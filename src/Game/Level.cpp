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

    setTarget({0,0});
    setTarget({0,4});
    setTarget({4,0});
    setTarget({4,4});

    addBox({2,1});
    addBox({1,2});
    addBox({2,3});
    addBox({3,2});
}

void Level::setTarget(const Math::Vector2<int> &vector) {
    level[vector.x()][vector.y()] = FieldType::Target;
}


FieldType Level::value(const Math::Vector2<int>& vector) {
    return level[vector.x()][vector.y()];
}

std::size_t Level::width() {
    return level.size();
}

std::size_t Level::height() {
    return level[0].size();
}

void Level::addBox(const Math::Vector2<int>& vector) {
    level[vector.x()][vector.y()] = FieldType::Box;

    boxes.push_back(new Box(vector));
}

void Level::moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to) {
    level[from.x()][from.y()] = FieldType::Empty;
    level[to.x()][to.y()] = FieldType::Box;

    box(from)->move(to-from);
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
