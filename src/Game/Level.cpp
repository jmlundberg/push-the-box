#include "Level.h"

#include <Math/Vector2.h>
#include <Swizzle.h>
#include <SceneGraph/Scene.h>

#include "FloorTile.h"
#include "WallBrick.h"

using namespace std;

namespace PushTheBox { namespace Game {

Level* Level::_current = nullptr;

Level* Level::current() {
    CORRADE_ASSERT(_current, "No Level instance created", nullptr);
    return _current;
}

Level::Level(const std::string&, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables) {
    CORRADE_ASSERT(!_current, "Only one Level instance can be created at a time", );
    _current = this;

    _size = {5, 5};

    level.resize(_size.product(), TileType::Empty);

    set({0, 1}, TileType::Floor, scene, drawables);
    set({0, 2}, TileType::Wall, scene, drawables);

    set({0, 0}, TileType::Target, scene, drawables);
    set({0, 4}, TileType::Target, scene, drawables);
    set({4, 0}, TileType::Target, scene, drawables);
    set({4, 4}, TileType::Target, scene, drawables);

    set({2, 1}, TileType::Box, scene, drawables);
    set({1, 2}, TileType::Box, scene, drawables);
    set({2, 3}, TileType::Box, scene, drawables);
    set({3, 2}, TileType::Box, scene, drawables);
}

Level::~Level() {
    CORRADE_INTERNAL_ASSERT(_current == this);
    _current = nullptr;
}

void Level::moveBox(const Math::Vector2<int>& from, const Math::Vector2<int>& to) {
    CORRADE_INTERNAL_ASSERT(at(from) == TileType::Box);

    at(from) = TileType::Floor;
    at(to) = TileType::Box;

    for(size_t i = 0; i < boxes.size(); ++i) {
        if(boxes[i]->position() != from) continue;

        boxes[i]->move(to-from);
        return;
    }

    CORRADE_INTERNAL_ASSERT(false);
}

void Level::set(const Math::Vector2<int>& position, TileType type, Scene3D* scene, SceneGraph::DrawableGroup<3>* drawables) {
    at(position) = type;

    Vector3 scenePosition = Vector3::from(swizzle<'x', '0', 'y'>(position));

    switch(type) {
        case TileType::Empty:
            break;
        case TileType::Box:
            boxes.push_back(new Box(position, scene, drawables));
            /* No break, as we need floor tile under it */
        case TileType::Floor:
            (new FloorTile(Color3<GLfloat>::fromHSV(0.0f,0.1f,0.3f), scene, drawables))
                ->translate(scenePosition);
            break;
        case TileType::Target:
            (new FloorTile(Color3<GLfloat>::fromHSV(0.0f, 1.0f, 0.5f), scene, drawables))
                ->translate(scenePosition);
            break;
        case TileType::Wall:
            (new WallBrick(Color3<GLfloat>::fromHSV(0.0f, 0.2f, 1.0f), scene, drawables))
                ->translate(scenePosition);
            break;
    }
}

}}
