# Resource compiler
add_executable(push-the-box-rc
    ResourceCompiler.cpp
    rc.cpp)
target_include_directories(push-the-box-rc PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_BINARY_DIR}/src)
target_link_libraries(push-the-box-rc PRIVATE
    Magnum::Magnum
    Magnum::MeshTools)
