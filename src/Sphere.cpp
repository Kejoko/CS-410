// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Sphere.h"

#include <iostream>
#include <sstream>
#include <string>

int Sphere::msCount = 0;

Sphere::Sphere(const std::string& line) : Object() {
    mId = msCount++;
    
    std::istringstream iss(line);
    iss >> mPosition(0) >> mPosition(1) >> mPosition(2);
    iss >> mRadius;
    iss >> mAmbientReflection(0) >> mAmbientReflection(1) >> mAmbientReflection(2);
    iss >> mDiffuseReflection(0) >> mDiffuseReflection(1) >> mDiffuseReflection(2);
}
