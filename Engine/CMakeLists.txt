cmake_minimum_required(VERSION 2.6)
project(Engine)

include_directories(${CMAKE_SOURCE_DIR}/Engine
                    ${SDL2_INCLUDE_DIRS})

if (NOT WIN32)
    add_definitions(-std=c++11)
endif()

file(GLOB SRC "src/*.cpp")
file(GLOB HEADERS "include/*.h")
add_library(engine STATIC ${SRC} ${HEADERS})
target_link_libraries(engine ${SDL2_LIBRARIES})
