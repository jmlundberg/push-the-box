cmake_minimum_required(VERSION 2.8.12)
project(PushTheBox CXX)

# CMake policies: enable MACOSX_RPATH by default
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()
# Don't treat imported targets with :: as files
if(POLICY CMP0028)
    cmake_policy(SET CMP0028 NEW)
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})

add_subdirectory(src)
