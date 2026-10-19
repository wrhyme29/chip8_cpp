cmake_minimum_required(VERSION 3.14)
project(chip8)
set(VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules")

find_package(GoogleTest)
find_package(SetupCPack)

add_subdirectory(Chip8)
