cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED $ENV{IDF_PATH})
  set(IDF_PATH "/opt/esp-idf/tools/cmake/project.cmake")
else()
  set(IDF_PATH $ENV{IDF_PATH}/tools/cmake/project.cmake)
endif()

include(${IDF_PATH})
#idf_build_set_property(MINIMAL_BUILD ON)


project(SillbirdRobot
  LANGUAGES CXX C
)

