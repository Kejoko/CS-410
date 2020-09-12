// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <fstream>
#include <iostream>
#include <string>

#include "DriverHandler.h"

/*
 Initialize the driver file name to be the name specified. Initialize the transformation
 matrix to be a 4x4 identity matrix. Initialize the vector of objects to be empty.
*/
DriverHandler::DriverHandler(const std::string& fileName) {
    mFileName = fileName;
}

/*
 This method is responsible for opening the driver file, reading the lines and handling
 them properly, then closing the file.
*/
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

/*
 This method performs operations on the transformation matrix according to the contents
 of the line from the driver.
*/
void DriverHandler::update_matrix(const std::string& line) {
    std::cout << line << '\n';
}

/*
 This method loads and creates an object from the specified object file then applies the
 transformation matrix to all of its vertices.
*/
void DriverHandler::load_object(const std::string& line) {
    std::cout << line << '\n';
}

/*
 This method saves the object to the file specified by the driver line.
*/
void DriverHandler::save_object(const std::string& line) {
    std::cout << line << '\n';
}
