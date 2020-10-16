// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "DriverHandler.h"

int main (int argc, char** argv) {
    
    if (argc != 3) return 1;
    
    DriverHandler DH(argv[1], argv[2]);
    DH.read_driver();
    
    return 0;
}
