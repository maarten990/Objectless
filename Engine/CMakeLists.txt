cmake_minimum_required(VERSION 2.6)
project(Engine)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
endif(UNIX)

include_directories(${CMAKE_SOURCE_DIR}/Engine
                    ${SDL2_INCLUDE_DIRS})

add_definitions(-std=c++11)

file(GLOB SRC "src/*.cpp")
add_library(engine STATIC ${SRC})
target_link_libraries(engine ${SDL2_LIBRARIES})
