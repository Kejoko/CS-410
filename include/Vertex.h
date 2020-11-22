// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include <Eigen/Dense>

class Vertex {
public:
    Vertex();
    
    Eigen::Vector3d mPosition;
    std::vector<int> mFaceIndices;
};

#endif //VERTEX_H
