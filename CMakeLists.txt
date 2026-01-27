cmake_minimum_required(VERSION 3.14)

project(netsim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(external/googletest)




# Pliki źródłowe
add_library(netsim_lib
    
    src/package.cpp
    src/nodes.cpp
    src/factory.cpp
    src/simulation.cpp
    src/reports.cpp
)

# Katalog z nagłówkami
target_include_directories(netsim_lib PUBLIC include)
# target_include_directories(netsim
#     PRIVATE
#         ${CMAKE_SOURCE_DIR}/include
# )


add_executable(netsim_tests
    test/main_gtest.cpp
    test/test_package.cpp
    test/test_nodes.cpp
)

target_link_libraries(netsim_tests
    netsim_lib
    gtest
    gtest_main
)

add_executable(netsim_app main.cpp)
target_link_libraries(netsim_app netsim_lib)

enable_testing()
add_test(NAME netsim_tests COMMAND netsim_tests)


target_compile_definitions(
    netsim_tests
    PUBLIC EXERCISE_ID=EXERCISE_ID_NODES
)