cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTRA_COMPONENT_DIRS src include)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
#idf_build_set_property(MINIMAL_BUILD ON)
project(SillbirdRobot)

