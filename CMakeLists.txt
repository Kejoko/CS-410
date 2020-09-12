#======================================================================
# Keegan Kochis
# kkochis@rams.colostate.edu
#======================================================================
cmake_minimum_required(VERSION 3.14)
project(Graphics VERSION 0.1 LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#======================================================================
# C++ 11 Support
#======================================================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================
# Include project dependencies
#======================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/dependencies)

#======================================================================
# Include project libraries
#======================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/src)

#======================================================================
# Subdirectories
#======================================================================
add_subdirectory(dependencies)
add_subdirectory(include)
add_subdirectory(src)

#=======================================s===============================
# Entry Point
#======================================================================
add_executable(render render.cpp)
