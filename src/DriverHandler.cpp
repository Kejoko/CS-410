// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "DriverHandler.h"

#include <math.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "Scene.h"

DriverHandler::DriverHandler(const std::string& inFileName, const std::string& outFileName) {
    mInFileName = inFileName;
    mOutFileName = outFileName;
    
    update_matrix("clear");
    update_cutoff_angle("90");
}





void DriverHandler::read_driver() {
    std::ifstream driverStream(mInFileName);
    std::string line;
    std::string word;
    
    while(getline(driverStream, line)) {
        if (!line.empty()) {
            std::istringstream lineReader(line);
            lineReader >> word;
            
            if (word == "recursionlevel"){
                mScene.update_recursion_level(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "camera") {
                mScene.create_camera(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "bounds") {
                mScene.update_bounds(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "res") {
                mScene.create_resolution(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "ambient") {
                mScene.create_ambient_light(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "light") {
                mScene.create_point_light(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "trans") {
                update_matrix(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "load") {
                mScene.create_object(line.substr(word.length() + 1, std::string::npos), mTransformationMatrix, mCuttoffAngle);
            }
            else if (word == "sphere") {
                mScene.create_sphere(line.substr(word.length() + 1, std::string::npos));
            }
            else if (word == "cutoffAngle") {
                update_cutoff_angle(line.substr(word.length() + 1, std::string::npos));
            }
        }
    }
    
    driverStream.close();
}





void DriverHandler::output_result() {
    mScene.render(mOutFileName);
}





void DriverHandler::update_matrix(const std::string& line) {
    if (line == "clear") {
        mTransformationMatrix <<    1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    0, 0, 0, 1;
    }
    else {
        std::istringstream infoStream(line);
        std::string type;
        double x, y, z;
        Eigen::Matrix4d modifier;
        
        infoStream >> type >> x >> y >> z;
        
        if (type == "move") {
            modifier << 1, 0, 0, x,
                        0, 1, 0, y,
                        0, 0, 1, z,
                        0, 0, 0, 1;
        }
        else if (type == "rota") {
            double theta;
            infoStream >> theta;
            
            Eigen::Vector3d W(x, y, z);
            W /= W.norm();
            
            Eigen::Vector3d M = W;
            double a = M(0);
            double b = M(1);
            double c = M(2);
            if (a != b && a != c && b != c) {
                if (a < b && a < c)
                    M(0) = 1;
                else if (b < a && b < c)
                    M(1) = 1;
                else if (c < a && c < b)
                    M(2) = 1;
            }
            else if (a == b && a != c && b != c) {
                if (a < c && b < c)
                    M(1) = 1;
                else if (c < a && c < b)
                    M(2) = 1;
            }
            else if (a != b && a == c && b != c) {
                if (a < b && c < b)
                    M(2) = 1;
                else if (b < a && b < c)
                    M(1) = 1;
            }
            else if (a != b && a != c && b == c) {
                if (a < b && a < c)
                    M(0) = 1;
                else if (b < a && c < a)
                    M(2) = 1;
            }
            else
                M(2) = 1;
            
            Eigen::Vector3d U = W.cross(M);
            U /= U.norm();
            
            Eigen::Vector3d V = W.cross(U);
            
            Eigen::Matrix4d Rw;
            Rw <<   U(0), U(1), U(2), 0,
                    V(0), V(1), V(2), 0,
                    W(0), W(1), W(2), 0,
                       0,    0,    0, 1;
            
            Eigen::Matrix4d Rz;
            double ct = cos((theta / 180.0) * M_PI);
            double st = sin((theta / 180.0) * M_PI);
            Rz <<   ct, -st, 0, 0,
                    st,  ct, 0, 0,
                     0,   0, 1, 0,
                     0,   0, 0, 1;
            
            modifier = Rw.transpose() * Rz * Rw;
        }
        else if (type == "scale") {
            modifier << x, 0, 0, 0,
                        0, y, 0, 0,
                        0, 0, z, 0,
                        0, 0, 0, 1;
        }
        
        mTransformationMatrix = modifier * mTransformationMatrix;
    }
}





void DriverHandler::update_cutoff_angle(const std::string& line) {
    std::istringstream iss(line);
    iss >> mCuttoffAngle;
}




//======================================================================
//======================================================================
//======================================================================
// EVERYTHING BELOW THIS POINT IS DEPRECATED AND UNUSED FROM P1
//======================================================================
//======================================================================
//======================================================================

//void DriverHandler::save_object(const std::string& line) {
//    std::string transformationFileName;
//    
//    size_t period = line.find('.');
//    if (period != std::string::npos) {
//        transformationFileName = line.substr(0, period+1) + "txt";
//    }
//    else {
//        transformationFileName = line + ".txt";
//    }
//    
//    size_t underscore = line.find('_');
//    if (underscore != std::string::npos) {
//        std::string end = transformationFileName.substr(underscore, std::string::npos);
//        transformationFileName = transformationFileName.substr(0, underscore) + "_transform" + end;
//    }
//    else {
//        transformationFileName = line.substr(0, period) + "_transform.txt";
//    }
//    
//    std::ofstream outTransformationFile(transformationFileName);
//    Object obj = mObjects.back();
//    mObjects.erase(mObjects.end());
//    
//    outTransformationFile << "# Transformation matrix\n" << mTransformationMatrix << "\n\n";
//    outTransformationFile << "# Sum absolute translations from original to transformed\n" << std::to_string(obj.sum_absolute_translations()) << "\n\n";
//    
//    obj.output(line);
//}
