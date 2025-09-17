cmake_minimum_required(VERSION 3.5)
project(kaspersky)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-DSCANNER_EXPORTS)
endif()

find_package(OpenSSL REQUIRED)
message(STATUS "Found OpenSSL: ${OPENSSL_VERSION}")

find_package(GTest REQUIRED)
message(STATUS "Found GTest: ${GTEST_VERSION}")

add_subdirectory(scanner_lib)
add_subdirectory(scanner_exe)
add_subdirectory(tests)