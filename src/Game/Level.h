#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "FieldType.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level {
    public:
        Level(std::string levelName);
        void setTarget(std::size_t x, std::size_t y);
        void addBox(std::size_t x, std::size_t y);

        std::vector<std::vector<FieldType>> level;
        std::vector<std::vector<bool>> isTarget;
};

}}

#endif
