// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

/*
 ASSIGNMENT QUESTIONS
 - Regarding the output of an obj file, should we assume there can be more than one obj file
   loaded? If so, how do we know which order to save them? On the first save command should
   we save the first obj file that was loaded?
 - Do we not need to report the vertex normals?
 - Can we assume faces will always be in the order of v1//vn1 v2//vn2 v3//vn3?
 - When do we output the file containing all of the transformation matrix information? Do we
   output it at the end of the driver file or when we output an object file?
 - Are we supposed to output a file containing the transformation matrix information, as the
   solution folders have? Are the transformation matrix files simply for debugging?
*/

#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>

class Object {
public:
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
