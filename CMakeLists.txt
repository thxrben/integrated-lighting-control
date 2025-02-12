### Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-fpermissive)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fpermissive")


cmake_minimum_required(VERSION 3.29)

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

# Loading ZMQ via PkgConfig:
# (ZMQPP does support cmake, so no need to find it!)
find_package(PkgConfig)
pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from above to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

#
# Generating static libraries:
#


#CONFIG
add_library(config STATIC src/config/Config.cpp)
target_include_directories(config PUBLIC ${PROJECT_SOURCE_DIR}/include)


# DMX ENGINE
add_library(effects STATIC src/effects/Effect.cpp src/effects/SineWave.cpp)
target_include_directories(effects PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(dmxoutput STATIC src/dmx_output/Output.cpp src/dmx_output/ArtNet.cpp)
target_include_directories(dmxoutput PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(fixture STATIC src/fixture/Fixture.cpp src/fixture/channel/Channel.cpp)
target_include_directories(fixture PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(dmxengine STATIC src/dmx_engine/DMXEngine.cpp src/dmx_engine/Universe.cpp)
target_include_directories(dmxengine PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(dmxengine PRIVATE effects fixture)

#NETWORKING
add_library(network STATIC src/network/NetworkManager.cpp)
target_include_directories(network PUBLIC ${PROJECT_SOURCE_DIR}/include config)
target_link_libraries(network PUBLIC config)

#SESSION
add_library(session STATIC src/session/Session.cpp src/session/SessionAPI.cpp src/session/SessionManager.cpp src/session/Station.cpp)
target_include_directories(session PUBLIC ${PROJECT_SOURCE_DIR}/include config)
target_link_libraries(session PRIVATE network config)

#LOGGING
add_library(logger STATIC src/logger/Log.cpp src/logger/LogEvent.cpp)
target_include_directories(logger PUBLIC ${PROJECT_SOURCE_DIR}/include)

#IPC
add_library(ipc STATIC src/ipc/IPCManager.cpp)
target_include_directories(ipc PUBLIC ${PROJECT_SOURCE_DIR}/include ${ZeroMQ_INCLUDE_DIR} config)
target_link_libraries(ipc PRIVATE ${ZeroMQ_LIBRARY} zmqpp config)   


#
# Create an executable
#

message("Generating executable...")

add_executable(ilc 
    src/main.cpp
)

target_link_libraries(ilc 
    PRIVATE
        config 
        ipc 
        logger
        dmxengine  
        session  
        network 
        Threads::Threads 
)

message("### CMake has completed! ###")