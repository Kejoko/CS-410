// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef SPHERE_H
#define SPHERE_H

#include <string>

#include <Eigen/Dense>

#include "Face.h"
#include "Object.h"
#include "Ray.h"

class Sphere: public Object {
public:
    static int msSphereCount;
    int mSphereId;
    
    Sphere() = default;
    Sphere(const std::string& line);
    
    double mRadius;
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mAmbientReflection;
    Eigen::Vector3d mDiffuseReflection;
    Eigen::Vector3d mSpecularReflection;
    Eigen::Vector3d mAttenuationReflection;
    
    double ray_intersect(Ray& ray, Face& bestFace) override;
};

#endif //SPHERE_H
