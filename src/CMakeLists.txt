find_package(Corrade REQUIRED Interconnect)

find_package(Magnum REQUIRED
    MeshTools
    Primitives
    SceneGraph
    Sdl2Application
    Shaders
    Shapes
    Text
    TextureTools)

if(CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED MagnumFont TgaImporter)
endif()

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

corrade_add_resource(PushTheBoxResources_RCS ../resources/resources.conf)
corrade_add_resource(PushTheBoxLevels_RCS ../levels/resources.conf)
corrade_add_resource(PushTheBoxShaders_RCS Shaders/resources.conf)

set(PushTheBox_SRCS
    Application.cpp

    Game/Box.cpp
    Game/Camera.cpp
    Game/FloorTile.cpp
    Game/Game.cpp
    Game/Hud.cpp
    Game/Player.cpp
    Game/Level.cpp
    Game/WallBrick.cpp

    Menu/Cursor.cpp
    Menu/Menu.cpp
    Menu/MenuItem.cpp

    Splash/Splash.cpp

    ResourceManagement/MeshResourceLoader.cpp
    Shaders/Blur.cpp

    ${PushTheBoxResources_RCS}
    ${PushTheBoxLevels_RCS}
    ${PushTheBoxShaders_RCS})

add_executable(push-the-box ${PushTheBox_SRCS})
target_include_directories(push-the-box PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})
if(CORRADE_TARGET_EMSCRIPTEN)
    target_link_libraries(push-the-box
        Magnum::MagnumFont
        Magnum::TgaImporter)
endif()
target_link_libraries(push-the-box
    Corrade::Interconnect
    Magnum::Magnum
    Magnum::MeshTools
    Magnum::Primitives
    Magnum::SceneGraph
    Magnum::Shaders
    Magnum::Shapes
    Magnum::Text
    Magnum::TextureTools
    Magnum::Application)

if(NOT CORRADE_TARGET_EMSCRIPTEN)
    add_subdirectory(ResourceManagement)
endif()

# Installation for Emscripten
if(CORRADE_TARGET_EMSCRIPTEN)
    install(FILES push-the-box.html DESTINATION ${MAGNUM_DEPLOY_PREFIX}/push-the-box RENAME index.html)
    install(FILES
        WebApplication.css EmscriptenApplication.js
        DESTINATION ${MAGNUM_DEPLOY_PREFIX}/push-the-box)
    install(TARGETS push-the-box DESTINATION ${MAGNUM_DEPLOY_PREFIX}/push-the-box)
endif()
