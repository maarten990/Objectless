cmake_minimum_required(VERSION 2.6)
project(Objectless)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
endif(UNIX)

# find Lua
find_package(Lua51)
if (LUA51_FOUND)
    option(ENABLE_LUA "Enables the Lua interface" ${LUA51_FOUND})
endif()

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/include
                    ${SDL2_INCLUDE_DIRS})

if (ENABLE_LUA)
    include_directories(${LUA_INCLUDE_DIR})
    add_definitions(-D WITH_LUA)
endif()

# compiler settings
add_definitions(-std=c++11)

# compiling
file(GLOB SRC "src/*.cpp")
if (ENABLE_LUA)
    list(APPEND SRC "src/lua/REPLSystem.cpp")
endif()

add_executable(objectless ${SRC})
target_link_libraries(objectless engine ${SDL2_LIBRARIES} ${SDLIMG_LIBRARIES})

if (ENABLE_LUA)
    target_link_libraries(objectless ${LUA_LIBRARIES})
endif()
