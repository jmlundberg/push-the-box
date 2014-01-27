find_package(Magnum REQUIRED
    MeshTools
    Primitives
    SceneGraph
    Shaders
    Shapes
    Text
    TextureTools)

if(CORRADE_TARGET_NACL)
    find_package(Magnum REQUIRED NaClApplication)
else()
    find_package(Magnum REQUIRED Sdl2Application)
endif()

# For newlib we need to link static plugins
if(CORRADE_TARGET_NACL_NEWLIB OR CORRADE_TARGET_EMSCRIPTEN)
    find_package(Magnum REQUIRED MagnumFont TgaImporter)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CORRADE_CXX_FLAGS}")
include_directories(${MAGNUM_INCLUDE_DIRS}
    ${MAGNUM_APPLICATION_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR})

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

if(CORRADE_TARGET_NACL_NEWLIB OR CORRADE_TARGET_EMSCRIPTEN)
    target_link_libraries(push-the-box
        ${MAGNUM_MAGNUMFONT_LIBRARIES}
        ${MAGNUM_TGAIMPORTER_LIBRARIES})
endif()

target_link_libraries(push-the-box
    ${CORRADE_INTERCONNECT_LIBRARIES}
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_MESHTOOLS_LIBRARIES}
    ${MAGNUM_PRIMITIVES_LIBRARIES}
    ${MAGNUM_SCENEGRAPH_LIBRARIES}
    ${MAGNUM_SHADERS_LIBRARIES}
    ${MAGNUM_SHAPES_LIBRARIES}
    ${MAGNUM_TEXT_LIBRARIES}
    ${MAGNUM_TEXTURETOOLS_LIBRARIES}
    ${MAGNUM_APPLICATION_LIBRARIES})

if(NOT CORRADE_TARGET_NACL AND NOT CORRADE_TARGET_EMSCRIPTEN AND NOT WIN32)
    add_subdirectory(ResourceManagement)
endif()

# Installation for NaCl
if(CORRADE_TARGET_NACL)
    install(FILES push-the-box-nacl.html WebApplication.css NaClApplication.js DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(TARGETS push-the-box DESTINATION ${CMAKE_INSTALL_PREFIX})

    if(CORRADE_TARGET_NACL_NEWLIB)
        install(FILES push-the-box-newlib.nmf RENAME push-the-box.nmf DESTINATION ${CMAKE_INSTALL_PREFIX})
    else()
        install(FILES push-the-box-glibc.nmf RENAME push-the-box.nmf DESTINATION ${CMAKE_INSTALL_PREFIX})
        include(UseNaCl)
        nacl_bundle_libs(${CMAKE_INSTALL_PREFIX}
            libppapi_cpp.so
            libpthread.so.a9de5678
            libstdc++.so.6
            libm.so.a9de5678
            libgcc_s.so.1
            libc.so.a9de5678
            libCorradeUtility.so.0 # just symlink
            libCorradeUtility.so.0.2
            libCorradePluginManager.so.0
            libCorradePluginManager.so.0.2 # just symlink
            libMagnum.so
            libMagnumMeshTools.so
            libMagnumPhysics.so
            libMagnumSceneGraph.so
            libMagnumShaders.so
            libdl.so.a9de5678
            libppapi_gles2.so
            runnable-ld.so
            PATHS "${NACL_TOOLCHAIN_PATH}/x86_64-nacl/lib" "${NACL_PREFIX}/lib")
    endif()
endif()

# Installation for Emscripten
if(CORRADE_TARGET_EMSCRIPTEN)
    install(FILES push-the-box-emscripten.html WebApplication.css EmscriptenApplication.js DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(TARGETS push-the-box DESTINATION ${CMAKE_INSTALL_PREFIX})
endif()
