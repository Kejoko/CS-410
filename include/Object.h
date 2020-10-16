// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>

class Object {
public:
    Object();
    Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix);
    
    std::string mFileName;
    std::vector<Eigen::Vector4d> mOldVertices;
    std::vector<Eigen::Vector4d> mVertices;
    std::vector<Eigen::Vector4d> mVertexNormals;
    std::vector<std::array<int, 9>> mFaces;
    std::string mSmoothing;
    std::vector<std::vector<int>> mLines;
    
    void handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_vertex_normal(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_smoothing(const std::string& info);
    void handle_face(const std::string& info);
    void handle_line(const std::string& info);
    
    double sum_absolute_translations();
    void output(const std::string& fileName);
};

#endif //OBJECT_H
