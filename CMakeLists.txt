cmake_minimum_required(VERSION 3.0)
project(alsa-volume-monitor)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(alsa-volume-monitor main.c)

find_package(ALSA REQUIRED)
include_directories(${ALSA_INCLUDE_DIRS})
target_link_libraries(alsa-volume-monitor ${ALSA_LIBRARIES})
