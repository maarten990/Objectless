cmake_minimum_required(VERSION 2.6)
project(Objectless)

# find SDL
if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_image)
    find_package(Box2D REQUIRED)
elseif(MSVC)
    set(SDL2_LOCATION "${CMAKE_SOURCE_DIR}" CACHE FILEPATH
        "The root folder of SDL2")
    set(SDL2_IMAGE_LOCATION "${CMAKE_SOURCE_DIR}" CACHE FILEPATH
        "The root folder of SDL2_image")
    set(BOX2D_LOCATION "${CMAKE_SOURCE_DIR}" CACHE FILEPATH
        "The root folder of Box2D")

    set(SDL2_LIBRARIES ${SDL2_LOCATION}/lib/x64/SDL2.lib
                       ${SDL2_LOCATION}/lib/x64/SDL2main.lib)
    set(SDL2_INCLUDE_DIRS ${SDL2_LOCATION}/include)

    set(SDLIMG_LIBRARIES ${SDL2_IMAGE_LOCATION}/lib/x64/SDL2_image.lib)
    set(SDLIMG_INCLUDE_DIRS ${SDL2_IMAGE_LOCATION}/include)

    set(BOX2D_LIBRARIES ${BOX2D_LOCATION}/lib/x64/$(Configuration)/Box2D.lib)
    set(BOX2D_INCLUDE_DIRS ${BOX2D_LOCATION})

# Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

# Stupid cmake sets an x86 linker flag even if generator is VS x64.
		if(CMAKE_EXE_LINKER_FLAGS MATCHES "machine:X86")
			string(REGEX REPLACE "machine:X86" "machine:X64" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
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
                    ${SDLIMG_INCLUDE_DIRS}
                    ${BOX2D_INCLUDE_DIRS})

if (ENABLE_LUA)
    include_directories(${LUA_INCLUDE_DIR})
    add_definitions(-D WITH_LUA)
endif()

# compiler settings
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
endif()

# compiling
file(GLOB SRC "src/*.cpp" "Engine/src/*.cpp")
file(GLOB_RECURSE ENGINE_SRC "Engine/src/*.cpp")
file(GLOB_RECURSE EXTERNAL_SRC "External/*.c" "External/*.cpp")
set(SRC ${SRC} ${ENGINE_SRC})
set(SRC ${SRC} ${EXTERNAL_SRC})

file(GLOB HEADERS "include/*.h")
file(GLOB_RECURSE ENGINE_HEADERS "Engine/*.h")
file(GLOB_RECURSE EXTERNAL_HEADERS "External/*.h")
set(HEADERS ${HEADERS} ${ENGINE_HEADERS})
set(HEADERS ${HEADERS} ${EXTERNAL_HEADERS})

if (ENABLE_LUA)
    file(GLOB LUA_HEADERS "include/lua/*.h")
    set(HEADERS ${HEADERS} ${LUA_HEADERS})
    list(APPEND SRC "src/lua/REPLSystem.cpp")
endif()

add_executable(objectless ${SRC} ${HEADERS})
target_link_libraries(objectless
                      ${SDL2_LIBRARIES}
                      ${SDLIMG_LIBRARIES}
                      ${BOX2D_LIBRARIES})

if (ENABLE_LUA)
    target_link_libraries(objectless ${LUA_LIBRARIES})
endif()
