#!/bin/bash

# This is a simple script to pack all of the necessary files together.
# The resulting tar file will end up on the desktop

# Tar the files
tar -cvf ~/Desktop/KeeganKochisXX.tar \
    dependencies \
    include \
    src \
    render.cpp \
    CMakeLists.txt \
    README.txt
