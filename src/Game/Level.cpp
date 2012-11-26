#include "Level.h"

using namespace std;

namespace PushTheBox { namespace Game {

Level::Level(std::string levelName) {
    //load levelu ze souboru podle jmena...zatim ukazkovy level na vyzkouseni...
    for(size_t i = 0; i != 5; ++i) {
        vector<FieldType> line;
        vector<bool> lineTarget;
        for(std::size_t j = 0; j < 5; ++j) {
            line.push_back(FieldType::Empty);
            lineTarget.push_back(false);
        }
        level.push_back(line);
        isTarget.push_back(lineTarget);
    }

    isTarget[0][0] = true;
    isTarget[0][4] = true;
    isTarget[4][0] = true;
    isTarget[4][4] = true;

    addBox(2,1);
    addBox(1,2);
    addBox(2,3);
    addBox(3,2);
}

void Level::setTarget(std::size_t x, std::size_t y) {
    if(x < level.size() && y < level[x].size()) {
        isTarget[x][y] = true;
    }
}

void Level::addBox(std::size_t x, std::size_t y) {
    if(x < level.size() && y < level[x].size()) {
        level[x][y] = FieldType::Box;
    }
}

}}
