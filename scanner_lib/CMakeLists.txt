cmake_minimum_required(VERSION 3.5)
project(scanner_lib)

add_library(scanner_lib SHARED
    scanner/scanner.cpp
    logger/logger.cpp
)

target_include_directories(scanner_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(scanner_lib OpenSSL::SSL OpenSSL::Crypto)

if(WIN32)
    target_compile_definitions(scanner_lib PRIVATE SCANNER_EXPORTS)
endif()