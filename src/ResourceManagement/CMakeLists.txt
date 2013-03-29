add_executable(push-the-box-rc
    ResourceCompiler.cpp
    rc.cpp)
target_link_libraries(push-the-box-rc
    ${MAGNUM_LIBRARIES}
    ${MAGNUM_MESHTOOLS_LIBRARIES})
