// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Sphere.h"

#include <math.h>

#include <cfloat>

#include <iostream>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "Face.h"
#include "Object.h"
#include "Ray.h"

int Sphere::msSphereCount = 0;

Sphere::Sphere(const std::string& line) : Object() {
    mSphereId = msSphereCount++;
    
    std::istringstream iss(line);
    iss >> mPosition(0) >> mPosition(1) >> mPosition(2);
    iss >> mRadius;
    iss >> mAmbientReflection(0) >> mAmbientReflection(1) >> mAmbientReflection(2);
    iss >> mDiffuseReflection(0) >> mDiffuseReflection(1) >> mDiffuseReflection(2);
    iss >> mSpecularReflection(0) >> mSpecularReflection(1) >> mSpecularReflection(2);
    iss >> mAttenuationReflection(0) >> mAttenuationReflection(1) >> mAttenuationReflection(2);
    iss >> mRefractionIndex;
}





void Sphere::ray_intersect(const Ray& ray, std::shared_ptr<Object>& pBestObject, Face& bestFace, double& bestBeta, double& bestGamma, double& bestT) {
    double v, r2, c2, d;
    double t = DBL_MAX;
    Eigen::Vector3d rtoc;
    
    rtoc = mPosition - ray.mPosition;
    
    v = rtoc.dot(ray.mDirection);
    
    c2 = rtoc.dot(rtoc);
    
    d = mRadius * mRadius - (c2 - (v * v));
    
    if (d > 0.0) {
        t = v - sqrt(d);
        
        if (t < bestT && t > 0.000001) {
            bestT = t;
            pBestObject = shared_from_this();
        }
    }
}





Eigen::Vector3d Sphere::calculate_refraction_t_vector(const Eigen::Vector3d& W, const Eigen::Vector3d& N, double eta1, double eta2, bool& success) {
    double etaR = eta1 / eta2;
    double aVal = -1 * etaR;
    double WN = W.dot(N);
    double radSq = (etaR * etaR) * (WN * WN - 1) + 1;
    
    Eigen::Vector3d T;
    std::cout << "radsq " << radSq << '\n';
    if (radSq < 0.0) {
        success = false;
        T(0) = 0.0;
        T(1) = 0.0;
        T(2) = 0.0;
    }
    else {
        success = true;
        double bVal = (etaR * WN) - sqrt(radSq);
        T = aVal * W + bVal * N;
    }
    
    std::cout << "T " << T(0) << ' ' << T(1) << ' ' << T(2) << '\n';
    return T;
}





Ray Sphere::calculate_refraction_exit_ray(const Eigen::Vector3d& W, const Eigen::Vector3d& surfaceNormal, const Eigen::Vector3d& point, double eta1, double eta2, bool& success) {
    Ray refractionRay;
    
    bool subSuccess;
    Eigen::Vector3d T1 = calculate_refraction_t_vector(W, surfaceNormal, eta2, eta1, subSuccess);
    
    if (!subSuccess) {
        success = false;
        return refractionRay;
    }
    success = true;
    
    Eigen::Vector3d normal = mPosition - point;
    std::cout << "weird " << normal(0) << ' ' << normal(1) << ' ' << normal(2) << '\n';
    
    Eigen::Vector3d exitPoint = point + 2 * normal.dot(T1) * T1;
    std::cout << "exit " << exitPoint(0) << ' ' << exitPoint(1) << ' ' << exitPoint(2) << '\n';
    
    Eigen::Vector3d normalIn = mPosition - exitPoint;
    normalIn = normalIn / normalIn.norm();
    std::cout << "nin " << normalIn(0) << ' ' << normalIn(1) << ' ' << normalIn(2) << '\n';
    
    Eigen::Vector3d T2 = calculate_refraction_t_vector(-1 * T1, normalIn, eta1, eta2, subSuccess);
    
    refractionRay.mPosition = exitPoint;
    refractionRay.mDirection = T2;
    return refractionRay;
}
