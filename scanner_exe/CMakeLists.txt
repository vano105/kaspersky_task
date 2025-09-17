cmake_minimum_required(VERSION 3.5)
project(scanner_exe)

add_executable(scanner_exe main.cpp)

target_include_directories(scanner_exe PRIVATE ${CMAKE_SOURCE_DIR}/scanner_lib)
target_link_libraries(scanner_exe scanner_lib)