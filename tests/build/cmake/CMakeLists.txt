project(miroir_tests)

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

set(SRC_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_executable(${PROJECT_NAME}
    ${SRC_FOLDER}/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/src)
