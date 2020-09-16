// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include <string>
#include <vector>

#include <Eigen/Dense>

class Object {
public:
    Object(const std::string& name);
    
    std::string mName;
    std::vector<Eigen::Vector4d> mVertices;
    std::vector<Eigen::Vector4d> mVertexNormals;
    std::vector<std::array<double, 9>> mFaces;
};

#endif //OBJECT_H
