set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wold-style-cast -Winit-self -pedantic -std=c++0x -fvisibility=hidden")

find_package(Magnum REQUIRED
    MeshTools
    Physics
    SceneGraph
    Shaders)

if(MAGNUM_TARGET_NACL)
    find_package(Magnum REQUIRED NaClApplication)
    set(APPLICATION_LIBRARY ${MAGNUM_NACLAPPLICATION_LIBRARIES})
else()
    find_package(Magnum REQUIRED Sdl2Application)
    set(APPLICATION_LIBRARY ${MAGNUM_SDL2APPLICATION_LIBRARIES})
endif()

include_directories(${MAGNUM_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

corrade_add_resource(PushTheBox_RCS PushTheBoxData
    ../resources/push-the-box.mesh ALIAS push-the-box.mesh
    ../resources/push-the-box.conf ALIAS push-the-box.conf)

set(PushTheBox_SRCS
    Application.cpp
    AbstractScreenedApplication.cpp

    Game/Box.cpp
    Game/FloorTile.cpp
    Game/Game.cpp
    Game/Player.cpp
    Game/WallBrick.cpp

    Menu/Menu.cpp
    Menu/MenuItem.cpp

    ResourceManagement/MeshResourceLoader.cpp

    ${PushTheBox_RCS})

add_executable(push-the-box ${PushTheBox_SRCS})
target_link_libraries(push-the-box
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_PHYSICS_LIBRARIES}
    ${MAGNUM_SCENEGRAPH_LIBRARIES}
    ${MAGNUM_SHADERS_LIBRARIES}
    ${APPLICATION_LIBRARY})

add_subdirectory(ResourceManagement)
