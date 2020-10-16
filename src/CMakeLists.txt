#======================================================================
# src
#======================================================================

add_library(
  Object
  Object.cpp)

add_library(
  Scene
  Scene.cpp)

add_library(
  DriverHandler
  DriverHandler.cpp)
target_link_libraries(
  DriverHandler
  Scene)