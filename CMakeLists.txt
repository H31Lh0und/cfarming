cmake_minimum_required(VERSION 3.10)
project(PlantGrowthSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(cfarming PlantGrowthSystem.cpp)
target_link_libraries(cfarming sfml-graphics sfml-window sfml-system)
