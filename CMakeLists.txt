cmake_minimum_required(VERSION 2.6)
project(Objectless)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
elseif(MSVC)
    set(SDL2_LOCATION "${CMAKE_SOURCE_DIR}" CACHE FILEPATH
        "The root folder of SDL2")
    set(SDL2_IMAGE_LOCATION "${CMAKE_SOURCE_DIR}" CACHE FILEPATH
        "The root folder of SDL2_image")

    set(SDL2_LIBRARIES ${SDL2_LOCATION}/lib/x86/SDL2.lib
                       ${SDL2_LOCATION}/lib/x86/SDL2main.lib)
    set(SDL2_INCLUDE_DIRS ${SDL2_LOCATION}/include)

    set(SDLIMG_LIBRARIES ${SDL2_IMAGE_LOCATION}/lib/x86/SDL2_image.lib)
    set(SDLIMG_INCLUDE_DIRS ${SDL2_IMAGE_LOCATION}/include)

# Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

# find Lua
find_package(Lua51)
if (LUA51_FOUND)
    option(ENABLE_LUA "Enables the Lua interface" ${LUA51_FOUND})
endif()

include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/include
                    ${SDL2_INCLUDE_DIRS}
                    ${SDLIMG_INCLUDE_DIRS})

if (ENABLE_LUA)
    include_directories(${LUA_INCLUDE_DIR})
    add_definitions(-D WITH_LUA)
endif()

# compiler settings
if (NOT WIN32)
    add_definitions(-std=c++11)
endif()

# compiling
file(GLOB SRC "src/*.cpp" "Engine/src/*.cpp")
file(GLOB HEADERS "include/*.h" "Engine/*.h")
if (ENABLE_LUA)
    list(APPEND SRC "src/lua/REPLSystem.cpp")
endif()

add_executable(objectless ${SRC} ${HEADERS})
target_link_libraries(objectless ${SDL2_LIBRARIES} ${SDLIMG_LIBRARIES})

if (ENABLE_LUA)
    target_link_libraries(objectless ${LUA_LIBRARIES})
endif()
