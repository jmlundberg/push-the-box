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
            return _size;
        }

        inline FieldType value(const Math::Vector2<int>& position) const {
            return level[position.y()*_size.x()+position.x()];
        }

        Box* box(const Math::Vector2<int>& position);

        void moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to);

    protected:
        void set(const Math::Vector2<int>& position, FieldType type);

    private:
        inline FieldType& at(const Math::Vector2<int>& position) {
            return level[position.y()*_size.x()+position.x()];
        }

        Math::Vector2<int> _size;
        std::vector<FieldType> level;
        std::vector<Box*> boxes;
};

}}

#endif
