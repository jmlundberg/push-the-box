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
        static Level* instance();

        Level(const std::string& name);
        void setTarget(const Math::Vector2<int>& vector);
        void addBox(const Math::Vector2<int>& vector);
        void moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to);

        inline Math::Vector2<int> size() const {
            return {int(level.size()), int(level[0].size())};
        }

        FieldType value(const Math::Vector2<int>& vector);

        Box* box(const Math::Vector2<int>& position);

    private:
        std::vector<std::vector<FieldType> > level;
        std::vector<Box* > boxes;
};

}}

#endif
