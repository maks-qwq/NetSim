cmake_minimum_required(VERSION 3.10)

project(netsim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Pliki źródłowe
add_executable(netsim
    main.cpp
    src/netsim.cpp
)

# Katalog z nagłówkami
target_include_directories(netsim
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
