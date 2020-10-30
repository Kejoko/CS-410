// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Face.h"

#include <Eigen/Dense>

#include "Material.h"
#include "Object.h"
#include "Ray.h"

Face::Face() {
    
}

void Face::ray_intersect(const Ray& ray, bool& hit, double& t) {
    Eigen::Vector3d Av = mpObject->mVertices[mVertexIndices[0]];
    Eigen::Vector3d Bv = mpObject->mVertices[mVertexIndices[1]];
    Eigen::Vector3d Cv = mpObject->mVertices[mVertexIndices[2]];
    
    Eigen::Vector3d rayToA = Av - ray.mPosition;
    
    Eigen::Matrix3d MMt;
    MMt <<  (Av-Bv)(0), (Av-Cv)(0), ray.mDirection(0),
            (Av-Bv)(1), (Av-Cv)(1), ray.mDirection(1),
            (Av-Bv)(2), (Av-Cv)(2), ray.mDirection(2);
    
    Eigen::Matrix3d MMi = MMt.inverse();
    
    Eigen::Vector3d result = MMi * rayToA;
    
    double beta = result(0);
    double gamma = result(1);
    t = result(2);
    if ((beta > 0.0) && (gamma > 0.0) && ((beta + gamma) < 1.0) && (t > 0.0)) {
        hit = true;
    }
}
