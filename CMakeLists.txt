cmake_minimum_required(VERSION 3.0)
project(GA)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -pedantic -O2 -g")
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

add_executable(GATest main.cpp)
target_link_libraries(GATest PUBLIC GALib ShipLib ProjectionLib RiskLib)
target_include_directories(GATest PUBLIC ${PROJECT_SOURCE_DIR}/src)