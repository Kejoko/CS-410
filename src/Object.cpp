// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Object.h"

#include <math.h>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "Material.h"

bool has_printed_mat = false;

Object::Object() {
    
}

Object::Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix) {
    std::cout << "--- Object: " << fileName << " ---\n";
    
    std::istringstream fileNameStream(fileName);
    fileNameStream >> mFileName;
    
    std::ifstream objReader(mFileName);
    std::string line, word;
    
    bool has_printed_v = false;
    
    while(getline(objReader, line)) {
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
            if (!has_printed_v) {
                std::cout << "\n\n----- ----- vertices ----- -----\n";
                for (int i = 0; i < mVertices.size(); i++) {
                    std::cout << "V " << i+1 << '\t';
                    for (int j = 0; j < 3; j++) {
                        std::cout << mVertices[i](j) << ' ';
                    }
                    std::cout << '\n';
                }
                std::cout << "----- ----- ----- ----- -----\n\n\n";
                has_printed_v = true;
            }
            update_current_material(line.substr(word.length() + 1, std::string::npos));
        }
        else if (word == "f") {
            handle_face(line.substr(2, std::string::npos));
        }
        else if (word == "l") {
            handle_line(line.substr(2, std::string::npos));
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
    
    if (!has_printed_mat) {
        std::cout << "----- TRANSFORMATION MATRIX -----\n" << transformationMatrix << "\n----- ----- ----- ----- -----\n\n";
        
        has_printed_mat = true;
    }
    
    std::cout << "VERTEX " << info << '\n';
    std::cout << "old ";
    for (int i = 0; i < 4; i++) {
        std::cout << mOldHomogeneousVertices.back()(i) << ' ';
    }
    std::cout << "\nhom ";
    for (int i = 0; i < 4; i++) {
        std::cout << mHomogeneousVertices.back()(i) << ' ';
    }
    std::cout << "\nreg ";
    for (int i = 0; i < 3; i++) {
        std::cout << mVertices.back()(i) << ' ';
    }
    std::cout << "\n\n";
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
    std::cout << "\n\n----- Material library: " << fileName << " -----\n";
    
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
    std::cout << "----- ----- ----- ----- ----- ----- -----\n\n\n";
}





void Object::update_current_material(const std::string& materialName) {
    std::cout << "-- Looking for material: " << materialName << " --\n";
    for (auto material : mMaterialLibrary) {
        if (material->mName == materialName) {
            mCurrentMaterial = material;
            std::cout << "\tFound " << material->mName << '\n';
            break;
        }
    }
}





void Object::handle_face(const std::string& info) {
    std::cout << "FACE: " << info << '\n';
    
    Face newFace;
    
    std::istringstream iss(info);
    std::string vertexInfo;
    while(getline(iss, vertexInfo, ' ')) {
        newFace.mVertexIndices.push_back(vertexInfo[0] - '0' - 1); // Convert numeric character into an integer
    }
    
    Eigen::Vector3d BA = (mHomogeneousVertices[newFace.mVertexIndices[1]] - mHomogeneousVertices[newFace.mVertexIndices[0]]).head<3>();
    Eigen::Vector3d CB = (mHomogeneousVertices[newFace.mVertexIndices[2]] - mHomogeneousVertices[newFace.mVertexIndices[1]]).head<3>();
    
    newFace.mNormal = BA.cross(CB);
    newFace.mNormal = newFace.mNormal / newFace.mNormal.norm();
    
    mFaces.push_back(newFace);
    
    std::cout << "A(" << mFaces.back().mVertexIndices[0] + 1 << "):\t";
    for (int i = 0; i < 4; i++) {
        std::cout << mHomogeneousVertices[mFaces.back().mVertexIndices[0]](i) << ' ';
    }
    std::cout << "\nB(" << mFaces.back().mVertexIndices[1] + 1 << "):\t";
    for (int i = 0; i < 4; i++) {
        std::cout << mHomogeneousVertices[mFaces.back().mVertexIndices[1]](i) << ' ';
    }
    std::cout << "\nC(" << mFaces.back().mVertexIndices[2] + 1 << "):\t";
    for (int i = 0; i < 4; i++) {
        std::cout << mHomogeneousVertices[mFaces.back().mVertexIndices[2]](i) << ' ';
    }
    
    std::cout << "\nBA:\t";
    for (int i = 0; i < 3; i++) {
        std::cout << BA(i) << ' ';
    }
    std::cout << "\nCB:\t";
    for (int i = 0; i < 3; i++) {
        std::cout << CB(i) << ' ';
    }
    
    std::cout << "\nNormal\n" << mFaces.back().mNormal << "\n\n";
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

