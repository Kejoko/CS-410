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
    
    void ray_intersect(const Ray& ray, std::shared_ptr<Object>& pBestObject, Face& bestFace, double& bestBeta, double& bestGamma, double& bestT) override;
    Eigen::Vector3d calculate_refraction_t_vector(const Eigen::Vector3d& W, const Eigen::Vector3d& N, double eta1, double eta2, bool& success);
    Ray calculate_refraction_exit_ray(const Eigen::Vector3d& W, const Eigen::Vector3d& surfaceNormal, const Eigen::Vector3d& point, double eta1, double eta2, bool& success);
    
};

#endif //SPHERE_H
