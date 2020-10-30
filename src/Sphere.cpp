// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Sphere.h"

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
}





double Sphere::ray_intersect(Ray& ray, Face& bestFace) {
    double v, r2, c2, d;
    double t = DBL_MAX;
    Eigen::Vector3d rtoc;
    
    rtoc = mPosition - ray.mPosition;
    
    v = rtoc.dot(ray.mDirection);
    
    c2 = rtoc.dot(rtoc);
    
    d = mRadius * mRadius - (c2 - (v * v));
    
    if (d > 0.0) {
        t = v - sqrt(d);
    }
    
    return t;
}
