// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "Object.h"

Object::Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix) {
    mFileName = fileName;
    
    std::ifstream objReader(fileName);
    std::string line, word;
    std::istringstream infoStream;
    
    while(getline(objReader, line)) {
        if (line.substr(0,1) == "v" && line.substr(0,2) != "vn") {
            handle_vertex(line.substr(2, std::string::npos), transformationMatrix);
        }
        else if (line.substr(0,2) == "vn") {
            handle_vertex_normal(line.substr(3, std::string::npos));
        }
        else if (line.substr(0,1) == "s") {
            handle_smoothing(line.substr(2, std::string::npos));
        }
        else if (line.substr(0,1) == "f") {
            handle_face(line.substr(2, std::string::npos));
        }
    }
    
    objReader.close();
}

void Object::handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix) {
    double x, y, z;
    std::istringstream infoStream(info);
    infoStream >> x >> y >> z;
    mVertices.emplace_back(x, y, z, 1.0);
    mVertices.back() = transformationMatrix * mVertices.back();
    std::cout << mVertices.back() << "\n\n";
}

void Object::handle_vertex_normal(const std::string& info) {
    std::cout << "VN " << info << '\n';
}

void Object::handle_smoothing(const std::string& info) {
    std::cout << "S " << info << '\n';
}

void Object::handle_face(const std::string& info) {
    std::cout << "F " << info << '\n';
}
