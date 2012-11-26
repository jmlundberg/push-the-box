#ifndef PushTheBox_Game_Level_h
#define PushTheBox_Game_Level_h

#include "FieldType.h"
#include "PushTheBox.h"
#include "Box.h"

#include <vector>
#include <string>

namespace PushTheBox { namespace Game {

class Level {
    public:
        Level(const std::string& name);
        void setTarget(const Math::Vector2<int>& vector);
        void addBox(const Math::Vector2<int>& vector);
        void moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to);

        FieldType value(const Math::Vector2<int>& vector);
        std::size_t width();
        std::size_t height();

        Box* box(const Math::Vector2<int>& position);

    private:
        std::vector<std::vector<FieldType> > level;
        std::vector<Box* > boxes;
};

}}

#endif
