// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef DRIVER_HANDLER_H
#define DRIVER_HANDLER_H

#include <string>
#include <vector>

/*
 This class is responsible for reading lines from the driver file and handling
 them properly. It has a member variable representing the transformation
 matrix and a vector of objects representing all of the objects in the scene.
 This class has functions to read in a line, and a function to handle each
 type of line.
*/
class DriverHandler {
public:
    DriverHandler(const std::string& fileName);
    
    std::string mFileName;
    //4x4 matrix type mTransformationMatrix;
    //std::vector<object type> mObjects;
    
    void read_driver();
    //void update_matrix(const std::string& line);
    //void update_objects(const std::string& line);
};

#endif //DRIVER_HANDLER_H
