// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "DriverHandler.h"

int main (int argc, char** argv) {
    
    DriverHandler DH(argv[1]);
    DH.read_driver();
    
    return 0;
}
