#!/bin/bash

# This is a simple script to pack all of the necessary files together.
# The resulting tar file will end up on the desktop

# Tar the files
tar -cvf ~/Desktop/KeeganKochis04.tar \
    dependencies \
    include \
    src \
    raytracer.cpp \
    CMakeLists.txt \
    README.txt \
    driver00.txt \
    driver01.txt \
    driver01.ppm \
    driver02.txt \
    driver02.ppm
