// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef SPHERE_H
#define SPHERE_H

#include <string>

#include <Eigen/Dense>

#include "Object.h"

class Sphere: public Object {
public:
    Sphere(const std::string& line);
    
    int mRadius;
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mAmbientReflection;
    Eigen::Vector3d mDiffuseReflection;
    Eigen::Vector3d mSpecularReflection;
    Eigen::Vector3d mAttenuationReflection;
};

#endif //SPHERE_H
