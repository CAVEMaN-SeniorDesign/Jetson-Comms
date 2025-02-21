cmake_minimum_required(VERSION 3.30)
project(JetsonCaveman LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
# Add global flags for other compilers here
endif()

# Creating this as a library called jetson_comms
add_library(jetson_comms SHARED src/jetson_cave_talk.cc)

# source already specified with the add_library cmd above
# target_sources(jetson_comms
#     PRIVATE
#         ${CMAKE_CURRENT_SOURCE_DIR}/src/jetson_cave_talk.cc
# )

# Note, CMake needs *_INTERACEs when installing as exportable library to replace the absolute path.
# target_include_directories(jetson_comms 
#     PUBLIC 
#         ${CMAKE_CURRENT_SOURCE_DIR}/inc
#         ${CMAKE_CURRENT_SOURCE_DIR}/CAVeTalk/lib/common/inc
# )

target_include_directories(jetson_comms 
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc> # $<BUILD_INTERFACE: part specifies to use this inc path only when building (doesn't install)
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/CAVeTalk/lib/common/inc> # same for CAVeTalk dependencies
        $<INSTALL_INTERFACE:include> # Will specify to look for headers in /install/include after installation instead of this /inc
)

install(
    TARGETS jetson_comms
    EXPORT jetson_commsConfig # exports a package config
    LIBRARY DESTINATION lib # adding built .so files to lib folder
    ARCHIVE DESTINATION lib # add .a for static files to lib as well
    RUNTIME DESTINATION bin # if jetson_comms is an executable, place in bin
)
install(
    EXPORT jetson_commsConfig
    DESTINATION lib/cmake/jetson_comms
)