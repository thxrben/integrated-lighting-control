### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

cmake_minimum_required(VERSION 3.15)

project(CustomDMXEngine VERSION 1.0
                        DESCRIPTION "Integrated Lighting Control"
                        LANGUAGES CXX)



set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(ZeroMQ REQUIRED)

#add_compile_options(-fsanitize=undefined,address)
#add_link_options(-fsanitize=undefined,address)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

message("-- The build directory is " ${CMAKE_BINARY_DIR} )

#message("-- Adding libraries/classes: ")
#target_include_directories(ilc PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(zmq STATIC IMPORTED)
add_library(zmqpp STATIC IMPORTED)

#
# Generating static libraries:
#

add_library(ipc STATIC 
    src/ipc/IPCManager.cpp
)

target_include_directories(ipc 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)


#
# Create an executable
#

message("Generating executable...")


add_executable(ilc 
    src/main.cpp
)

#Link Libraries to libraries
target_link_libraries(ipc
        PRIVATE
        zmq:zmqpp
)   

target_link_libraries(ilc 
    PRIVATE 
        ipc 
        effects
        dmxengine 
        config 
        network 
        log 
        session 
        dmx_outputs 
        fixture 
        channel 
        Threads::Threads 
)

message("### CMake has completed! ###")