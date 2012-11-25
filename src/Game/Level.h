#ifndef Level_h
#define Level_h

#include "FieldType.h"

#include <vector>
#include <string>

namespace PushTheBox{ namespace Game{

class Level{

public:
    Level(std::string levelName);
    void setTarget(size_t x, size_t y);
    void addBox(size_t x, size_t y);

    std::vector<std::vector<FieldType> > level;
    std::vector<std::vector<bool> > isTarget;
};

}}

#endif // Level_h
