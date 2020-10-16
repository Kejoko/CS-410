// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include <iostream>

#include "DriverHandler.h"

int main (int argc, char** argv) {
    
    if (argc != 3) {
        std::cerr << "ERROR: Expected 3 arguments. Recieved " << argc << std::endl;
        return 1;
    }
    
    DriverHandler DH(argv[1], argv[2]);
    DH.read_driver();
    DH.output_result();
    
    return 0;
}
