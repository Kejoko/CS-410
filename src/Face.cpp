// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Face.h"

#include <math.h>

#include <iostream>

#include <Eigen/Dense>

#include "Material.h"
#include "Object.h"
#include "Ray.h"
#include "Vertex.h"

bool PRINT = false;

Face::Face() {
    
}





void Face::calculate_average_vertex_normals() {
    int count;
    double cos_theta, theta;
    Eigen::Vector3d sum;
    Vertex* current_vertex;
    Face* current_face;
    for (int i = 0; i < mVertexIndices.size(); i++) {
        count = 0;
        sum(0) = 0.0;
        sum(1) = 0.0;
        sum(2) = 0.0;
        
        current_vertex = &(mpObject->mVertices[mVertexIndices[i]]);
        
        for (int j = 0; j < current_vertex->mFaceIndices.size(); j++) {
            current_face = &(mpObject->mFaces[current_vertex->mFaceIndices[j]]);
            
            cos_theta = mNormal.dot(current_face->mNormal);
            theta = acos(cos_theta) * (180.0 / M_PI);
            
            if (theta <= mpObject->mCutoffAngle) {
                sum += current_face->mNormal;
                count++;
            }
        }
        
        sum /= count;
        sum = sum / sum.norm();
        
        mAverageVertexNormals.push_back(sum);
    }
}





void Face::ray_intersect(const Ray& ray, bool& hit, double& t, double& beta, double& gamma) {
    Eigen::Vector3d Av = mpObject->mVertices[mVertexIndices[0]].mPosition;
    Eigen::Vector3d Bv = mpObject->mVertices[mVertexIndices[1]].mPosition;
    Eigen::Vector3d Cv = mpObject->mVertices[mVertexIndices[2]].mPosition;
    
    Eigen::Vector3d rayToA = Av - ray.mPosition;
    
    Eigen::Matrix3d MMt;
    MMt <<  (Av-Bv)(0), (Av-Cv)(0), ray.mDirection(0),
            (Av-Bv)(1), (Av-Cv)(1), ray.mDirection(1),
            (Av-Bv)(2), (Av-Cv)(2), ray.mDirection(2);
    
    Eigen::Matrix3d MMi = MMt.inverse();
    
    Eigen::Vector3d result = MMi * rayToA;
    
    beta = result(0);
    gamma = result(1);
    t = result(2);
    if ((beta > 0.0) && (gamma > 0.0) && ((beta + gamma) < 1.0) && (t > 0.0)) {
        hit = true;
        
//        std::cout << "----- HIT -----\t" << t << '\n';
//        std::cout << "Av " << Av(0) << ' ' << Av(1) << ' ' << Av(2) << '\n';
//        std::cout << "Bv " << Bv(0) << ' ' << Bv(1) << ' ' << Bv(2) << '\n';
//        std::cout << "Cv " << Cv(0) << ' ' << Cv(1) << ' ' << Cv(2) << '\n';
//        std::cout << "Dv " << ray.mDirection(0) << ' ' << ray.mDirection(1) << ' ' << ray.mDirection(2) << '\n';
//        std::cout << "MMt\n" << MMt << '\n';
    }
}





Eigen::Vector3d Face::calculate_point_normal(double beta, double gamma) {
    Eigen::Vector3d normal;
    
    normal = (1.0 - beta - gamma) * mAverageVertexNormals[0] + beta * mAverageVertexNormals[1] + gamma * mAverageVertexNormals[2];
    
//    normal = normal / normal.norm();
    
    if (PRINT) {
        std::cout << "beta  " << beta << "\ngamma " << gamma << '\n';
        std::cout << "sumN  " << normal(0) << ' ' << normal(1) << ' ' << normal(2) << '\n';
    }
    
    return normal;
}
