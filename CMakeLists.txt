cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.1)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQIRED)

add_executable(allocator main.cpp)

add_executable(tests tests.cpp)

set_target_properties(allocator tests PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
)

set_target_properties(tests PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIR}"
)

target_link_libraries(allocator
    Threads::Threads
)

target_link_libraries(tests
    ${Boost_LIBRARIES}
)

enable_testing()

add_test(utest tests)