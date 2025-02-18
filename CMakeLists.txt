cmake_minimum_required(VERSION 3.30)
project(JetsonCaveman LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
# Add global flags for other compilers here
endif()

target_sources(jetson_comms
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/jetson_cave_talk.cc
)

target_include_directories(jetson_comms 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/CAVeTalk/lib/common/inc
)


