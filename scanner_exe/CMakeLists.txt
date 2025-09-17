cmake_minimum_required(VERSION 3.5)
project(scanner)

add_executable(scanner main.cpp)

target_include_directories(scanner PRIVATE ${CMAKE_SOURCE_DIR}/scanner_lib)
target_link_libraries(scanner scanner_lib)