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
    ../resources/push-the-box.conf ALIAS push-the-box.conf
    ../levels/0.txt ALIAS levels/0.txt
    ../levels/1.txt ALIAS levels/1.txt)

set(PushTheBox_SRCS
    Application.cpp
    AbstractScreenedApplication.cpp

    Game/Box.cpp
    Game/Camera.cpp
    Game/FloorTile.cpp
    Game/Game.cpp
    Game/Player.cpp
    Game/Level.cpp
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

if(NOT MAGNUM_TARGET_NACL)
    add_subdirectory(ResourceManagement)
endif()

# Installation for NaCl
if(MAGNUM_TARGET_NACL)
    install(FILES index.html push-the-box.nmf DESTINATION ${CMAKE_INSTALL_PREFIX})
    install(TARGETS push-the-box DESTINATION ${CMAKE_INSTALL_PREFIX})

    include(NaClMacros)
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
