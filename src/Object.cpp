// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Object.h"

#include <math.h>

#include <cfloat>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "Face.h"
#include "Material.h"
#include "Ray.h"

int Object::msObjectCount = 0;

Object::Object() {
    
}

Object::Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix, double cutoffAngle) {
    mObjectId = msObjectCount++;
    
    mCutoffAngle = cutoffAngle;
    
    std::istringstream fileNameStream(fileName);
    fileNameStream >> mFileName;
    
    std::ifstream objReader(mFileName);
    std::string line, word;
    
    bool has_printed_v = false;
    
    while(getline(objReader, line)) {
        if (!line.empty()) {
            std::istringstream iss(line);
            iss >> word;
            
            if (word == "v") {
                handle_vertex(line.substr(2, std::string::npos), transformationMatrix);
            }
            else if (word == "vn") {
                handle_vertex_normal(line.substr(3, std::string::npos), transformationMatrix);
            }
            else if (word == "s") {
                handle_smoothing(line.substr(2, std::string::npos));
            }
            else if (word == "mtllib") {
                create_material_library(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "usemtl") {
                update_current_material(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "f") {
                handle_face(line.substr(2, std::string::npos));
            }
            else if (word == "l") {
                handle_line(line.substr(2, std::string::npos));
            }
        }
    }
    
    objReader.close();
}





void Object::handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix) {
    std::istringstream infoStream(info);
    double x, y, z;
    
    infoStream >> x >> y >> z;
    mOldHomogeneousVertices.emplace_back(x, y, z, 1.0);
    mHomogeneousVertices.emplace_back(x, y, z, 1.0);
    
    mHomogeneousVertices.back() = transformationMatrix * mHomogeneousVertices.back();
    
    Eigen::Vector3d updatedVertex = mHomogeneousVertices.back().head<3>();
    updatedVertex = updatedVertex / mHomogeneousVertices.back()(3);
    mVertices.emplace_back(updatedVertex);
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





void Object::create_material_library(const std::string& fileName) {
    std::ifstream materialLibraryStream(fileName);
    std::string line;
    std::string word;
    
    while(getline(materialLibraryStream, line)) {
        if (!line.empty()) {
            std::istringstream lineReader(line);
            lineReader >> word;
            
            if (word == "newmtl") {
                std::shared_ptr<Material> newMaterial = std::make_shared<Material>(materialLibraryStream, line.substr(word.length() + 1, std::string::npos));
                mMaterialLibrary.push_back(newMaterial);
            }
        }
    }
    
    materialLibraryStream.close();
}





void Object::update_current_material(const std::string& materialName) {
    for (auto material : mMaterialLibrary) {
        if (material->mName == materialName) {
            mCurrentMaterial = material;
            break;
        }
    }
}





void Object::handle_face(const std::string& info) {
    Face newFace;
    
    std::istringstream iss(info);
    std::string vertexInfo;
    int vertexIndex;
    while(getline(iss, vertexInfo, ' ')) {
        std::istringstream vertexIndexer(vertexInfo);
        vertexIndexer >> vertexIndex;
        newFace.mVertexIndices.push_back(vertexIndex - 1);
    }
    
    Eigen::Vector3d BA = (mHomogeneousVertices[newFace.mVertexIndices[1]] - mHomogeneousVertices[newFace.mVertexIndices[0]]).head<3>();
    Eigen::Vector3d CB = (mHomogeneousVertices[newFace.mVertexIndices[2]] - mHomogeneousVertices[newFace.mVertexIndices[1]]).head<3>();
    
    newFace.mNormal = BA.cross(CB);
    newFace.mNormal = newFace.mNormal / newFace.mNormal.norm();
    
    newFace.mMaterial = mCurrentMaterial;
    
    newFace.mpObject = this;
    
    mFaces.push_back(newFace);
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





void Object::ray_intersect(const Ray& ray, std::shared_ptr<Object>& pBestObject, Face& bestFace, double& bestT) {
    bool hit;
    double t;
    
    Eigen::Vector3d A, B, C, rtoa, result;
    Eigen::Matrix3d MM, MMinverse;
    
    for (auto currFace : mFaces) {
        hit = false;
        currFace.ray_intersect(ray, hit, t);
        if (hit) {
            if (t > 0.000001 && t < bestT) {
                pBestObject = shared_from_this();
                bestT = t;
                bestFace = currFace;
            }
        }
    }
}





double Object::sum_absolute_translations() {
    double sum = 0;
    double difference, xDiff, yDiff, zDiff;
    
    for (size_t i = 0; i < mVertices.size(); i++) {
        
        xDiff = fabs(mVertices[i](0) - mOldHomogeneousVertices[i](0));
        yDiff = fabs(mVertices[i](1) - mOldHomogeneousVertices[i](1));
        zDiff = fabs(mVertices[i](2) - mOldHomogeneousVertices[i](2));
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
//    for (size_t i = 0; i < mFaces.size(); i++) {
//        outFile << "f ";// << std::resetiosflags(std::ios::showbase);
//
//        if (mFaces[i][2] == 0 && mFaces[i][5] == 0 && mFaces[i][8] == 0) {
//            outFile << mFaces[i][0] << ' '
//                    << mFaces[i][3] << ' '
//                    << mFaces[i][6] << '\n';
//        }
//        else {
//            for (int j = 0; j < 9; j++) {
//                value = mFaces[i][j];
//                if (value != 0) outFile << value;
//
//                if ((j == 0 || j == 1) || (j == 3 || j==4) || (j == 6 || j == 7)) outFile << '/';
//                else if (j == 2 || j == 5) outFile << ' ';
//                else outFile << '\n';
//            }
//        }
//    }
    
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

