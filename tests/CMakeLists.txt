cmake_minimum_required(VERSION 3.10)

# Create test executable
add_executable(search_engine_tests
    index_tests.cpp
    server_tests.cpp
    ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
    ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
)

# Add include directories
target_include_directories(search_engine_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Link with GTest and other dependencies
target_link_libraries(search_engine_tests PRIVATE 
    gtest 
    gtest_main
    nlohmann_json::nlohmann_json
)

# Add tests
include(GoogleTest)
gtest_discover_tests(search_engine_tests)
