// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <math.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "DriverHandler.h"

DriverHandler::DriverHandler(const std::string& fileName) {
    mFileName = fileName;
    mTransformationMatrix <<    1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1;
}





void DriverHandler::read_driver() {
    std::ifstream driverStream(mFileName);
    std::string line;
    
    while(getline(driverStream, line)) {
        if (line.substr(0,5) == "trans")
            update_matrix(line.substr(6, line.length() - 6));
        else if (line.substr(0,4) == "load")
            load_object(line.substr(5, line.length() - 5));
        else if (line.substr(0,4) == "save")
            save_object(line.substr(5, line.length() - 5));
    }
    
    driverStream.close();
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
            if (M(0) < M(1) && M(0) < M(2))
                M(0) = 1;
            else if (M(1) < M(0) && M(1) < M(2))
                M(1) = 1;
            else if (M(2) < M(0) && M(2) < M(1))
                M(2) = 1;
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





void DriverHandler::load_object(const std::string& line) {
    mObjects.emplace_back(line, mTransformationMatrix);
}





void DriverHandler::save_object(const std::string& line) {
    std::string transformationFileName = "transformation_.txt";
    std::ofstream outTransformationFile(transformationFileName);
    Object obj = mObjects[0];
    mObjects.erase(mObjects.begin());
    double* sums = obj.sum_absolute_translations();
    
    outTransformationFile << "# Transformation matrix\n" << mTransformationMatrix << "\n\n";
    outTransformationFile << "# Inverse transformation matrix\n" << mTransformationMatrix.inverse() << "\n\n";
    outTransformationFile << "# Sum absolute translations from original to transformed\n" << std::to_string(sums[0]) << "\n\n";
    outTransformationFile << "# Sum absolute translations from original to transformed to \"original\"\n" << std::to_string(sums[1]) << '\n';
    
    obj.output(line);
}
