#include "Level.h"

#include <sstream>
#include <Utility/Resource.h>
#include <Math/Vector2.h>
#include <Swizzle.h>
#include <SceneGraph/Scene.h>

#include "FloorTile.h"
#include "WallBrick.h"
#include "Game/Box.h"

namespace PushTheBox { namespace Game {

Level::Level(const std::string& name, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables, SceneGraph::AnimableGroup<3>* animables): Object3D(scene), _name(name), _remainingTargets(0) {
    /* Get level data */
    Corrade::Utility::Resource rs("PushTheBoxData");
    std::istringstream in(rs.get("levels/" + name + ".txt"));

    /* Level size on first line */
    in >> _size.x() >> _size.y();
    level.resize(_size.product(), TileType::Empty);
    CORRADE_ASSERT((_size > Vector2i(3, 3)).all(), "Level" << name << "is too small:" << _size, );

    if(in.peek() == '\r')
        in.ignore();

    CORRADE_INTERNAL_ASSERT(in.peek() == '\n');
    in.ignore();

    /* Next level name on second line */
    std::getline(in, _nextName);
    if(_nextName[_nextName.size()-1] == '\r')
        _nextName = _nextName.substr(0, _nextName.size()-1);

    /* Sanity checks */
    std::size_t boxCount = 0;
    std::size_t targetCount = 0;

    /* Parse the file */
    _playerPosition = {-1, -1};
    Vector2i position;
    while(in.peek() > 0) {
        TileType type = {};
        switch(in.peek()) {
            /* Empty, already marked */
            case ' ': break;

            /* Wall */
            case '#': type = TileType::Wall; break;

            /* Starting position */
            case '@':
                CORRADE_ASSERT(_playerPosition == Vector2i(-1, -1), "Multiple starting positions in level" << name, );
                _playerPosition = position;
                /* No break, as we need to mark it as floor */

            /* Floor */
            case '_': type = TileType::Floor; break;

            /* Box */
            case '$':
                type = TileType::Box;
                ++boxCount;
                break;

            /* Starting position on target */
            case '+':
                CORRADE_ASSERT(_playerPosition == Vector2i(-1, -1), "Multiple starting positions in level" << name, );
                _playerPosition = position;
                /* No break, as we need to mark it as target */

            /* Target */
            case '.':
                type = TileType::Target;
                ++targetCount;
                ++_remainingTargets;
                break;

            /* Box on target */
            case '*':
                type = TileType::BoxOnTarget;
                ++boxCount;
                ++targetCount;
                break;

            case '\r':
                in.ignore();
                CORRADE_INTERNAL_ASSERT(in.peek() == '\n');
                /*no break*/

            /* New line */
            case '\n':
                in.ignore();
                position.x() = 0;
                ++position.y();
                continue;

            default:
                CORRADE_ASSERT(false, "Unknown character" << in.peek() << "in file of level" << name << "at position" << position, );
        }

        in.ignore();
        set(position, type, drawables, animables);
        ++position.x();
    }

    /* Sanity checks */
    CORRADE_ASSERT(_remainingTargets != 0, "Level is already solved", );
    CORRADE_ASSERT(_playerPosition != Vector2i(-1, -1), "Level" << name << "has no starting position", );
    CORRADE_ASSERT(boxCount == targetCount, "Level" << name << "has" << boxCount << "boxes, but" << targetCount << "targets", );
}

void Level::movePlayer(const Vector2i& direction) {
    CORRADE_INTERNAL_ASSERT(direction.dot() == 1);
    Vector2i newPosition = _playerPosition + direction;

    /* Cannot move out of map */
    if((newPosition < Vector2i()).any() || (newPosition >= size()).any())
        return;

    /* Pushing box */
    if(at(newPosition) == TileType::Box ||
       at(newPosition) == TileType::BoxOnTarget) {
        Vector2i newBoxPosition = _playerPosition + direction*2;

        /* Cannot push box out of map */
        if((newBoxPosition < Vector2i()).any() || (newBoxPosition >= size()).any())
            return;

        /* The box can be pushed only on the floor */
        if(at(newBoxPosition) != TileType::Floor &&
           at(newBoxPosition) != TileType::Target)
            return;

        /* Move the box */
        Box* box = nullptr;
        for(std::size_t i = 0; i < boxes.size(); ++i) {
            if(boxes[i]->position == newPosition) {
                box = boxes[i];
                break;
            }
        }
        CORRADE_INTERNAL_ASSERT(box);

        box->translate(Vector3(swizzle<'x', '0', 'y'>(direction)));
        box->position += direction;

        if(at(newPosition) == TileType::BoxOnTarget) {
            ++_remainingTargets;
            at(newPosition) = TileType::Target;
        } else at(newPosition) = TileType::Floor;

        if(at(newBoxPosition) == TileType::Target) {
            --_remainingTargets;
            at(newBoxPosition) = TileType::BoxOnTarget;
            if(box->type != Box::Type::OnTarget) {
                box->type = Box::Type::OnTarget;
                box->movedToTarget();
            }
        } else {
            at(newBoxPosition) = TileType::Box;
            if(box->type != Box::Type::OnFloor) {
                box->type = Box::Type::OnFloor;
                box->movedFromTarget();
            }
        }

    /* Other than that we can move on the floor, but nowhere else */
    } else if(at(newPosition) != TileType::Floor &&
              at(newPosition) != TileType::Target)
        return;

    /* Move the player */
    _playerPosition = newPosition;
}

void Level::set(const Vector2i& position, TileType type, SceneGraph::DrawableGroup<3>* drawables, SceneGraph::AnimableGroup<3>* animables) {
    at(position) = type;

    switch(type) {
        case TileType::Empty:
            break;
        case TileType::Box:
            boxes.push_back(new Box(position, Box::Type::OnFloor, this, drawables, animables));
            /* No break, as we need floor tile under it */
        case TileType::Floor:
            new FloorTile(position, FloorTile::Type::Floor, this, drawables);
            break;
        case TileType::BoxOnTarget:
            boxes.push_back(new Box(position, Box::Type::OnTarget, this, drawables, animables));
            /* No break, as we need target tile under it */
        case TileType::Target:
            new FloorTile(position, FloorTile::Type::Target, this, drawables);
            break;
        case TileType::Wall:
            new WallBrick(position, this, drawables);
            break;
    }
}

}}
