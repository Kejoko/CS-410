#!/bin/bash

# This is a simple script to pack all of the necessary files together.
# The resulting tar file will end up on the desktop

# Tar the files
tar -cvf ~/Desktop/KeeganKochis05.tar \
    dependencies \
    include \
    src \
    raytracer.cpp \
    CMakeLists.txt \
    README.txt \
    cloud* \
    fence* \
    hand* \
    my* \
    skybox* \
    scene* \
    reference.png \
    
