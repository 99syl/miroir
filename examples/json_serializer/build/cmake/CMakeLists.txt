project(miroir_json_serializer_example)

cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)

include(load_nlohmann_json.cmake)

add_executable(json_serializer ${CMAKE_CURRENT_SOURCE_DIR}/../../src/main.cpp)
target_include_directories(json_serializer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/src)
target_link_libraries(json_serializer nlohmann_json)
