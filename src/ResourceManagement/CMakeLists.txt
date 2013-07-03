# Resource compiler
add_executable(push-the-box-rc
    ResourceCompiler.cpp
    rc.cpp)
target_link_libraries(push-the-box-rc
    ${MAGNUM_MESHTOOLS_LIBRARIES}
    ${MAGNUM_LIBRARIES})

# Font converter
find_package(Magnum REQUIRED WindowlessGlxApplication)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resourceManagementConfigure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/resourceManagementConfigure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(push-the-box-font-converter FontConverter.cpp)
target_link_libraries(push-the-box-font-converter
    ${MAGNUM_TEXT_LIBRARIES}
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_WINDOWLESSAPPLICATION_LIBRARIES})
