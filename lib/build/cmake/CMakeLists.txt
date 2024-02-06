project(miroir)

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

add_library(miroir INTERFACE)
target_include_directories(miroir INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
