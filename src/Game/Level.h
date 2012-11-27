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

        inline Math::Vector2<int> size() const {
            return {int(level.size()), int(level[0].size())};
        }

        inline FieldType value(const Math::Vector2<int>& position) const {
            return level[position.x()][position.y()];
        }

        Box* box(const Math::Vector2<int>& position);

        void moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to);

    protected:
        void set(const Math::Vector2<int>& position, FieldType type);

    private:
        std::vector<std::vector<FieldType> > level;
        std::vector<Box* > boxes;
};

}}

#endif
