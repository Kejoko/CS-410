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
    double mRefractionIndex;
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mAmbientReflection;
    Eigen::Vector3d mDiffuseReflection;
    Eigen::Vector3d mSpecularReflection;
    Eigen::Vector3d mAttenuationReflection;
    
    void ray_intersect(const Ray& ray, std::shared_ptr<Object>& pBestObject, Face& bestFace, double& bestT) override;
};

#endif //SPHERE_H
