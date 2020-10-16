// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef DRIVER_HANDLER_H
#define DRIVER_HANDLER_H

#include <string>
#include <vector>

#include <Eigen/Dense>

#include "Object.h"
#include "Scene.h"

/*
 This class is responsible for reading lines from the driver file and handling
 them properly. It has a member variable representing the transformation
 matrix and a vector of objects representing all of the objects in the scene.
 This class has functions to read in a line, and a function to handle each
 type of line.
*/
class DriverHandler {
public:
    DriverHandler(const std::string& inFileName, const std::string& outFileName);
    
    std::string mInFileName;
    std::string mOutFileName;
    
    Scene mScene;
    
    Eigen::Matrix4d mTransformationMatrix;
    std::vector<Object> mObjects;
    
    void read_driver();
    void update_matrix(const std::string& line);
    void load_object(const std::string& line);
    void save_object(const std::string& line);
};

#endif //DRIVER_HANDLER_H
