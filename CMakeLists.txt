cmake_minimum_required(VERSION 3.10)
project(The_Last_Train CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB CORE_SOURCES "src/core/*.cpp")

add_executable(last_train ${CORE_SOURCES})

set_property(TARGET last_train PROPERTY
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
