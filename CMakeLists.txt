#======================================================================
# Keegan Kochis
# kkochis@rams.colostate.edu
#======================================================================
cmake_minimum_required(VERSION 3.11)
project(Graphics VERSION 0.1 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#======================================================================
# C++ 11 Support
#======================================================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "-Ofast -Wall -Werror -Wextra -Wpedantic")

#======================================================================
# Include project dependencies
#======================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/dependencies)

#======================================================================
# Include project libraries
#======================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include)

#======================================================================
# Subdirectories
#======================================================================
add_subdirectory(src)

#======================================================================
# Entry Point
#======================================================================
list(APPEND LIBS DriverHandler Object)

add_executable(raytracer raytracer.cpp)

target_link_libraries(
  raytracer
  PRIVATE
  ${LIBS})
