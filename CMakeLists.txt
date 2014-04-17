cmake_minimum_required(VERSION 2.6)
project(Objectless)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_search_module(SDL2 REQUIRED sdl2)
endif(UNIX)

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${SDL2_INCLUDE_DIRS})

# compiler settings
add_definitions(-std=c++11)

# compiling
file(GLOB SRC "src/*.cpp")
add_executable(objectless ${SRC})
target_link_libraries(objectless ${SDL2_LIBRARIES})
