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
        if (line.substr(0,1) == "v" && line.substr(0,2) != "vn" && line.substr(0,2) != "vt") {
            handle_vertex(line.substr(2, std::string::npos), transformationMatrix);
        }
        else if (line.substr(0,2) == "vn") {
            handle_vertex_normal(line.substr(3, std::string::npos), transformationMatrix);
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
    std::istringstream infoStream(info);
    double x, y, z;
    
    infoStream >> x >> y >> z;
    mVertices.emplace_back(x, y, z, 1.0);
    
    mVertices.back() = transformationMatrix * mVertices.back();
}





void Object::handle_vertex_normal(const std::string& info, const Eigen::Matrix4d& transformationMatrix) {
    std::istringstream infoStream(info);
    double x, y, z;
    
    infoStream >> x >> y >> z;
    mVertexNormals.emplace_back(x, y, z, 1.0);
    
    mVertexNormals.back() = transformationMatrix * mVertexNormals.back();
}





void Object::handle_smoothing(const std::string& info) {
    mSmoothing = info;
}





void Object::handle_face(const std::string& info) {
    std::istringstream infoStream(info);
    std::string face, token;
    int index = 0;
    
    std::array<int, 9> faceValues;
    
    while(getline(infoStream, face, ' ')) {
        std::istringstream secondaryStream(face);
        
        while(getline(secondaryStream, token, '/')) {
            faceValues[index] = (token.empty()) ? 0 : std::stoi(token);
            index++;
        }
    }
    
    mFaces.push_back(faceValues);
}





void Object::output(const std::string& fileName) {
    
}
