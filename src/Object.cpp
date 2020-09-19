// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <math.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "Object.h"

Object::Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix) {
    std::istringstream fileNameStream(fileName);
    fileNameStream >> mFileName;
    
    std::ifstream objReader(mFileName);
    std::string line, word;
    std::istringstream infoStream;
    
    while(getline(objReader, line)) {
        if (line.substr(0,1) == "v" && line.substr(0,2) != "vn" && line.substr(0,2) != "vp" && line.substr(0,2) != "vt") {
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
        else if (line.substr(0,1) == "l") {
            handle_line(line.substr(2, std::string::npos));
        }
    }
    
    objReader.close();
}





void Object::handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix) {
    std::istringstream infoStream(info);
    double x, y, z;
    
    infoStream >> x >> y >> z;
    mOldVertices.emplace_back(x, y, z, 1.0);
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
    std::string index, token;
    int i = 0, tokenCount = 0;
    
    std::array<int, 9> faceValues;
    
    while(getline(infoStream, index, ' ')) {
        std::istringstream secondaryStream(index);
        
        while(getline(secondaryStream, token, '/')) {
            faceValues[i] = (token.empty()) ? 0 : std::stoi(token);
            i++;
            tokenCount++;
        }
    }
    
    if (tokenCount == 3) {
        faceValues[3] = faceValues[1];
        faceValues[6] = faceValues[2];
        
        faceValues[1] = 0;
        faceValues[2] = 0;
        
        faceValues[4] = 0;
        faceValues[5] = 0;
        
        faceValues[7] = 0;
        faceValues[8] = 0;
    }
    
    mFaces.push_back(faceValues);
}




void Object::handle_line(const std::string& info) {
    std::vector<int> line;
    std::istringstream infoStream(info);
    int index;
    
    while (infoStream >> index) {
        line.push_back(index);
    }
    
    mLines.push_back(line);
}





double Object::sum_absolute_translations() {
    double sum = 0;
    double difference, xDiff, yDiff, zDiff;
    
    for (size_t i = 0; i < mVertices.size(); i++) {
        
        xDiff = fabs(mVertices[i](0) - mOldVertices[i](0));
        yDiff = fabs(mVertices[i](1) - mOldVertices[i](1));
        zDiff = fabs(mVertices[i](2) - mOldVertices[i](2));
        difference = xDiff + yDiff + zDiff;
        
        sum += difference;
    }
    
    return sum;
}





void Object::output(const std::string& fileName) {
    std::ofstream outFile(fileName);
    
    outFile << "# Blender v2.79 (sub 0) OBJ File: ''\n";
    outFile << "# www.blender.org\n";
    
    for (size_t i = 0; i < mVertices.size(); i++) {
        outFile << "v "
                << std::to_string(mVertices[i](0)) << ' '
                << std::to_string(mVertices[i](1)) << ' '
                << std::to_string(mVertices[i](2)) << '\n';
    }
    
    /*
    for (size_t i = 0; i < mVertexNormals.size(); i++) {
        
    }
    */
    
    outFile << "s " << mSmoothing << '\n';
    
    int value;
    for (size_t i = 0; i < mFaces.size(); i++) {
        outFile << "f " << std::resetiosflags(std::ios::showbase);
        
        if (mFaces[i][2] == 0 && mFaces[i][5] == 0 && mFaces[i][8] == 0) {
            outFile << mFaces[i][0] << ' '
                    << mFaces[i][3] << ' '
                    << mFaces[i][6] << '\n';
        }
        else {
            for (int j = 0; j < 9; j++) {
                value = mFaces[i][j];
                if (value != 0) outFile << value;
                
                if ((j == 0 || j == 1) || (j == 3 || j==4) || (j == 6 || j == 7)) outFile << '/';
                else if (j == 2 || j == 5) outFile << ' ';
                else outFile << '\n';
            }
        }
    }
    
    for (size_t i = 0; i < mLines.size(); i++) {
        outFile << "l ";
        for (size_t j = 0; j < mLines[i].size(); j++) {
            outFile << mLines[i][j];
            if (j != mLines[i].size() - 1)
                outFile << ' ';
        }
        outFile << '\n';
    }
    
    outFile.close();
}

