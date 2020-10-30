// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef FACE_H
#define FACE_H

#include <memory>
#include <vector>

#include <Eigen/Dense>

#include "Material.h"
#include "Ray.h"

class Object;

class Face {
public:
    Face();
    
    std::vector<int> mVertexIndices;
    Eigen::Vector3d mNormal;
    std::shared_ptr<Material> mMaterial;
    
    Object* mpObject;
    
    void ray_intersect(const Ray&, bool& hit, double& t);
};;

#endif //FACE_H
