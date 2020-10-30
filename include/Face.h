// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef FACE_H
#define FACE_H

#include <memory>
#include <vector>

#include <Eigen/Dense>

#include "Material.h"

struct Face_t {
    std::vector<int> mVertexIndices;
    
    Eigen::Vector3d mNormal;
    
    std::shared_ptr<Material> mMaterial;
}; typedef Face_t Face;

#endif //FACE_H
