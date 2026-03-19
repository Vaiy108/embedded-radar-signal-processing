cmake_minimum_required(VERSION 3.10)

project(embedded_radar_signal_processing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(radar_demo
    src/main.cpp
    src/radar_pipeline.cpp
)