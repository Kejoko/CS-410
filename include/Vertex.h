// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
//#include <unordered_map>

#include <Eigen/Dense>

class Vertex {
public:
    Vertex();
    
    Eigen::Vector3d mPosition;
    std::vector<int> mFaceIndices;
//    std::unordered_map<int, Eigen::Vector3d> mAverageNormals;
};

#endif //VERTEX_H
