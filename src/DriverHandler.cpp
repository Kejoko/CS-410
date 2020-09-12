// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <math.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <Eigen/Dense>

#include "DriverHandler.h"

DriverHandler::DriverHandler(const std::string& fileName) {
    mFileName = fileName;
    mTransformationMatrix <<    1, 0, 0, 0,
                                0, 1, 0, 0,
                                0, 0, 1, 0,
                                0, 0, 0, 1;
    //mObjects = blank vector
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
    std::cout << line << '\n';
    
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
            
            //Calculate Rz, the rotation matrix about the z axis
            
            Eigen::Vector3d W(x, y, z);
            W = W / sqrt(x*x + y*y + z*z);
            
            Eigen::Vector3d M = W;
            if (M(0) < M(1) && M(0) < M(2))
                M(0) = 1;
            else if (M(1) < M(0) && M(1) < M(2))
                M(1) = 1;
            else if (M(2) < M(0) && M(2) < M(1))
                M(2) = 1;
            M = M / sqrt( M(0)*M(0) + M(1)*M(1) + M(2)*M(2) );
            
            Eigen::Vector3d U = W.cross(M);
            
            Eigen::Vector3d V = W.cross(U);
            
            Eigen::Matrix4d Rw;
            Rw <<   U(0), U(1), U(2), 0,
                    V(0), V(1), V(2), 0,
                    W(0), W(1), W(2), 0,
                       0,    0,    0, 1;
            
            //Calculate Rt
            
            //modifer = Rt * Rz * Rw
        }
        else if (type == "scale") {
            modifier << x, 0, 0, 0,
                        0, y, 0, 0,
                        0, 0, z, 0,
                        0, 0, 0, 1;
        }
        
        mTransformationMatrix = mTransformationMatrix * modifier;
    }
    
    std::cout << mTransformationMatrix << '\n';
}

void DriverHandler::load_object(const std::string& line) {
    std::cout << line << '\n';
}

void DriverHandler::save_object(const std::string& line) {
    std::cout << line << '\n';
}
