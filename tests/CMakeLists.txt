cmake_minimum_required(VERSION 3.5)
project(tests)

add_executable(tests
    main.cpp
)

target_link_libraries(tests
    GTest::gtest_main
    scanner_lib
    OpenSSL::SSL
    OpenSSL::Crypto
)

target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/scanner_lib)

include(GoogleTest)
gtest_discover_tests(tests)