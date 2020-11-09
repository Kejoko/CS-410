// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Material.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <Eigen/Dense>

Material::Material(std::ifstream& inFileStream, const std::string& nameLine) {
    std::istringstream nameStream(nameLine);
    nameStream >> mName;
    
    std::string line, type;
    while (getline(inFileStream, line)) {
        if (is_complete()) {
            break;
        }
        
        std::istringstream iss(line);
        iss >> type;
        
        if (type == "Ns") {
            iss >> mSpecularExponent;
        }
        else if (type == "Ka") {
            iss >> mAmbient(0) >> mAmbient(1) >> mAmbient(2);
        }
        else if (type == "Kd") {
            iss >> mDiffuse(0) >> mDiffuse(1) >> mDiffuse(2);
        }
        else if (type == "Ks") {
            iss >> mSpecular(0) >> mSpecular(1) >> mSpecular(2);
            mReflectance(0) = mSpecular(0);
            mReflectance(1) = mSpecular(1);
            mReflectance(2) = mSpecular(2);
        }
        else if (type == "Ke") {
            // Not using this for project 3
        }
        else if (type == "Ni") {
            // Not using this for project 3
            iss >> mOpticalDensity;
        }
        else if (type == "d") {
            // Not using this for project 3
            iss >> mDissolve;
        }
        else if (type == "illum") {
            iss >> mIllumination;
        }
    }
}





bool Material::is_complete() {
    bool nameFull = !mName.empty();
    
    bool specExFull = mSpecularExponent != -1;
    
    bool ambientFull = true;
    bool diffuseFull = true;
    bool specularFull = true;
    bool reflectanceFull = true;
    for (int i = 0; i < 3; i++) {
        if (mAmbient(i) < 0.0 || mAmbient(i) > 1.0)
            ambientFull = false;
        if (mDiffuse(i) < 0.0 || mDiffuse(i) > 1.0)
            diffuseFull = false;
        if (mSpecular(i) < 0.0 || mSpecular(i) > 1.0)
            specularFull = false;
        if (mReflectance(i) < 0.0 || mReflectance(i) > 1.0)
            reflectanceFull = false;
    }
    
//    bool optDenseFull = mOpticalDensity >= 0.001 && mOpticalDensity <= 10.0;
//
//    bool dissolveFull = mDissolve >= 0.0 && mDissolve <= 10.0;
    bool optDenseFull = true;
    bool dissolveFull = true;
    
    bool illumFull = mIllumination >= 0 && mIllumination <= 10;
    
//    std::cout << "nameFull      " << nameFull << '\n';
//    std::cout << "specExFull    " << specExFull << '\n';
//    std::cout << "ambientFull   " << ambientFull << '\n';
//    std::cout << "diffuseFull   " << diffuseFull << '\n';
//    std::cout << "specularFull  " << specularFull << '\n';
//    std::cout << "illumFull     " << illumFull << '\n';
    
    return nameFull && specExFull && ambientFull && diffuseFull && specularFull && reflectanceFull && optDenseFull && dissolveFull && illumFull;
}
