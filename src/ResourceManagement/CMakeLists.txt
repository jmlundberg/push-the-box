# Resource compiler
add_executable(push-the-box-rc
    ResourceCompiler.cpp
    rc.cpp)
target_link_libraries(push-the-box-rc
    ${MAGNUM_MESHTOOLS_LIBRARIES}
    ${MAGNUM_LIBRARIES})
