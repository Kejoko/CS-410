// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <fstream>
#include <iostream>
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
    std::cout << mTransformationMatrix << '\n';
    
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
}

void DriverHandler::load_object(const std::string& line) {
    std::cout << line << '\n';
}

void DriverHandler::save_object(const std::string& line) {
    std::cout << line << '\n';
}
