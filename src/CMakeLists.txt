#======================================================================
# src
#======================================================================
add_library(
  Material
  Material.cpp)

add_library(
  Object
  Object.cpp)
target_link_libraries(
  Object
  Material)

add_library(
  Sphere
  Sphere.cpp)
target_link_libraries(
  Sphere
  Object)

add_library(
  Scene
  Scene.cpp)
target_link_libraries(
  Scene
  Object
  Sphere)

add_library(
  DriverHandler
  DriverHandler.cpp)
target_link_libraries(
  DriverHandler
  Scene)