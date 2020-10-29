// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef MATERIAL_H
#define MATERIAL_H

#include <sstream>
#include <string>

#include <Eigen/Dense>

class Material {
public:
    Material();
    Material(std::ifstream& inFileStream, const std::string& name);
    
    std::string mName;
    double mSpecularExponent = -1;
    Eigen::Vector3d mAmbient;
    Eigen::Vector3d mDiffuse;
    Eigen::Vector3d mSpecular;
    double mOpticalDensity = -1;
    double mDissolve = -1;
    int mIllumination = -1;
    
    bool is_complete();
};

#endif //MATERIAL_H
