cmake_minimum_required(VERSION 3.22)
project(high_load_line)

set(CMAKE_CXX_STANDARD 14)

add_executable(high_load_line main.cpp)
find_package(OpenCV REQUIRED)
target_link_libraries(high_load_line ${OpenCV_LIBS})
