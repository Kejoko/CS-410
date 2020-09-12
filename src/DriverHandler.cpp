// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <fstream>
#include <iostream>
#include <string>

#include "DriverHandler.h"

DriverHandler::DriverHandler(const std::string& fileName) {
    mFileName = fileName;
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
