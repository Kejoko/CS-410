// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

/*
 ASSIGNMENT QUESTIONS
 - Do we need to contain comments in the output? Can we ignore them while reading them in?
 - Regarding the output of an obj file, should we assume there can be more than one obj file
   loaded? If so, how do we know which order to save them? On the first save command should
   we save the first obj file that was loaded?
 - Do we need to store the smoothing directive, textures, and material information?
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
    std::vector<Eigen::Vector4d> mVertices;
    std::vector<Eigen::Vector4d> mVertexNormals;
    std::vector<std::array<int, 9>> mFaces;
    //std::vector<int (*) [9]> mFaces;
    //std::vector<int*> mFaces;
    std::string mSmoothing;
    std::vector<int> mLine;
    
    void handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_vertex_normal(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_smoothing(const std::string& info);
    void handle_face(const std::string& info);
    
    void output(const std::string& fileName);
};

#endif //OBJECT_H
