include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

add_executable(push-the-box-rc
    ResourceCompiler.cpp
    rc.cpp)
target_link_libraries(push-the-box-rc
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_MESHTOOLS_LIBRARIES})
