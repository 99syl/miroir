project(miroir_basic_usage_example)

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../../src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/src)
