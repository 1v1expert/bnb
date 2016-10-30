cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME bnb)
project(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories("3rdparty/bandit" "3rdparty/json")
set(SOURCE_FILES main.cpp Test/Test.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})