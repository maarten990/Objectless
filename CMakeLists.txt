cmake_minimum_required(VERSION 2.6)
project(Objectless)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
endif(UNIX)

# find Lua
find_package(Lua51 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/include
                    ${SDL2_INCLUDE_DIRS}
                    ${LUA_INCLUDE_DIR})

# compiler settings
add_definitions(-std=c++11)

# compiling
file(GLOB SRC "src/*.cpp")
add_executable(objectless ${SRC})
target_link_libraries(objectless engine ${SDL2_LIBRARIES} ${SDLIMG_LIBRARIES}
    ${LUA_LIBRARIES})
