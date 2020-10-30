// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Scene.h"

#include <math.h>

#include <cfloat>

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Object.h"
#include "Ray.h"
#include "Sphere.h"

void Scene::create_resolution(const std::string& line) {
    std::istringstream iss(line);
    iss >> mImageWidth >> mImageHeight;
}





void Scene::update_recursion_level(const std::string& line) {
    std::istringstream iss(line);
    iss >> mRecursionLevel;
}





void Scene::create_camera(const std::string& line) {
    std::istringstream iss(line);
    double ex, ey, ez, lx, ly, lz, ux, uy, uz;
    iss >> ex >> ey >> ez >> lx >> ly >> lz >> ux >> uy >> uz >> mCamera.mNearClippingPlane;
    
    mCamera.mEyePosition(0) = ex;
    mCamera.mEyePosition(1) = ey;
    mCamera.mEyePosition(2) = ez;
    
    mCamera.mLookAt(0) = lx;
    mCamera.mLookAt(1) = ly;
    mCamera.mLookAt(2) = lz;
    
    mCamera.mUpVector(0) = ux;
    mCamera.mUpVector(1) = uy;
    mCamera.mUpVector(2) = uz;
    
    mCamera.mWDirection = mCamera.mEyePosition - mCamera.mLookAt;
    mCamera.mWDirection = mCamera.mWDirection / mCamera.mWDirection.norm();
    
    mCamera.mUDirection = mCamera.mUpVector.cross(mCamera.mWDirection);
    mCamera.mUDirection = mCamera.mUDirection / mCamera.mUDirection.norm();
    
    mCamera.mVDirection = mCamera.mWDirection.cross(mCamera.mUDirection);
    mCamera.mVDirection = mCamera.mVDirection / mCamera.mVDirection.norm();
}





void Scene::update_bounds(const std::string& line) {
    std::istringstream iss(line);
    iss >> mCamera.mLeftBound >> mCamera.mRightBound >> mCamera.mBottomBound >> mCamera.mTopBound;
}





void Scene::create_ambient_light(const std::string& line) {
    std::istringstream iss(line);
    double r, g, b;
    iss >> r >> g >> b;
    
    mAmbientLight.mColor(0) = r;
    mAmbientLight.mColor(1) = g;
    mAmbientLight.mColor(2) = b;
}





void Scene::create_point_light(const std::string& line) {
    std::istringstream iss(line);
    double x, y, z, w;
    double r, g, b;
    iss >> x >> y >> z >> w >> r >> g >> b;
    
    PointLight newLight;
    
    newLight.mPosition(0) = x;
    newLight.mPosition(1) = y;
    newLight.mPosition(2) = z;
    
    newLight.mColor(0) = r;
    newLight.mColor(1) = g;
    newLight.mColor(2) = b;
    
    mPointLights.push_back(newLight);
}




void Scene::create_sphere(const std::string& line) {
    std::shared_ptr<Object> newSphere = std::make_shared<Sphere>(line);
    mpObjects.push_back(newSphere);
}





void Scene::create_object(const std::string& line, const Eigen::Matrix4d& transformationMatrix) {
    std::shared_ptr<Object> newObject = std::make_shared<Object>(line, transformationMatrix);
    mpObjects.push_back(newObject);
}





Eigen::Vector3d convert_to_255(const Eigen::Vector3d& rgb) {
    Eigen::Vector3d rgbUpdated;
    
    rgbUpdated(0) = int(rgb(0) * 255 + 0.5);
    rgbUpdated(1) = int(rgb(1) * 255 + 0.5);
    rgbUpdated(2) = int(rgb(2) * 255 + 0.5);
    
    for (int i = 0; i < 3; i++) {
        rgbUpdated(i) = int(rgb(i) * 255 + 0.5);
        
        if (rgbUpdated(i) > 255) rgbUpdated(i) = 255;
        if (rgbUpdated(i) < 0) rgbUpdated(i) = 0;
    }
    
    return rgbUpdated;
}




Ray Scene::determine_pixelray(int pixw, int pixh) {
    Ray ray;
    
    double xOffset = pixw / (mImageWidth - 1) * (mCamera.mRightBound - mCamera.mLeftBound) + mCamera.mLeftBound;
    double yOffset = pixh / (mImageHeight - 1) * (mCamera.mBottomBound - mCamera.mTopBound) + mCamera.mTopBound;
    
    ray.mPosition = mCamera.mEyePosition + (xOffset * mCamera.mUDirection) + (yOffset * mCamera.mVDirection) + (mCamera.mNearClippingPlane * mCamera.mWDirection);
    
    ray.mDirection = ray.mPosition - mCamera.mEyePosition;
    ray.mDirection = ray.mDirection / ray.mDirection.norm();
    
    return ray;
}




Eigen::Vector3d Scene::raytrace(Ray& ray) {
    Eigen::Vector3d color(0, 0, 0);
    
    std::shared_ptr<Object> pBestObject = nullptr;
    Face face, bestFace;
    Eigen::Vector3d bestPoint;
    bool hit = false;
    double t;
    double tBest = DBL_MAX;
    
    for (auto pObject : mpObjects) {
        t = pObject->ray_intersect(ray, face);
        if (t > 0.0 && t < tBest) {
            tBest = t;
            pBestObject = pObject;
            bestFace = face;
            bestPoint = ray.mPosition + t * ray.mDirection;
            hit = true;
        }
    }
    
    if (hit) {
        Eigen::Vector3d surfaceNormal(1, 0 ,0);
        Eigen::Vector3d matAmbient(1, 1, 1);
        Eigen::Vector3d matDiffuse(1, 1, 1);
        Eigen::Vector3d matSpecular(1, 1, 1);
        double specularExponent;
        int illumination;
        
        std::shared_ptr<Sphere> pSphere = std::dynamic_pointer_cast<Sphere>(pBestObject);
        if (pSphere) {
            std::cout << "Best: Sphere " << pSphere->mSphereId << '\n';
            
            surfaceNormal = bestPoint - pSphere->mPosition;
            surfaceNormal = surfaceNormal / surfaceNormal.norm();
            
            matAmbient = pSphere->mAmbientReflection;
            matDiffuse = pSphere->mDiffuseReflection;
            matSpecular = pSphere->mSpecularReflection;
            specularExponent = 16;
            illumination = 3;
        }
        else {
            std::cout << "Best: Object " << pBestObject->mObjectId << '\n';
            
            surfaceNormal = bestFace.mNormal;
            
            matAmbient = bestFace.mMaterial->mAmbient;
            matDiffuse = bestFace.mMaterial->mDiffuse;
            matSpecular = bestFace.mMaterial->mSpecular;
            specularExponent = bestFace.mMaterial->mSpecularExponent;
            illumination = bestFace.mMaterial->mIllumination;
        }
        
        color(0) = mAmbientLight.mColor(0) * matAmbient(0);
        color(1) = mAmbientLight.mColor(1) * matAmbient(1);
        color(2) = mAmbientLight.mColor(2) * matAmbient(2);
        
        Eigen::Vector3d ptol;
        double surfaceProjection = -1;
        
        for (auto light : mPointLights) {
            ptol = light.mPosition - bestPoint;
            ptol = ptol / ptol.norm();
            
            surfaceProjection = surfaceNormal.dot(ptol);
            if (surfaceProjection > 0.0) {
                color(0) += light.mColor(0) * matDiffuse(0) * surfaceProjection;
                color(1) += light.mColor(1) * matDiffuse(1) * surfaceProjection;
                color(2) += light.mColor(2) * matDiffuse(2) * surfaceProjection;
            }
        }
    }
    
    std::cout << "Result\n" << convert_to_255(color) << '\n';
    return convert_to_255(color);
}





void Scene::render(const std::string& imageName) {
    std::cout << "\n\n----- ----- ----- -----\n";
    std::cout << "-----  Rendering  -----\n";
    std::cout << "----- ----- ----- -----\n\n\n";
    
    std::ofstream output(imageName);
    Eigen::Vector3d pixelColors;
    
    output << "P3\n";
    output << mImageWidth << ' ' << mImageHeight << ' ' << "255\n";
    for (int i = 0; i < mImageHeight; i++) {
        for (int j = 0; j < mImageWidth; j++) {
            std::cout << i << " , " << j << '\n';
            Ray ray = determine_pixelray(j, i);
            pixelColors = raytrace(ray);
            std::cout << "\n\n\n";
            
            output << pixelColors(0) << ' ' << pixelColors(1) << ' ' << pixelColors(2);
            
            if (j < mImageHeight - 1) {
                output << ' ';
            }
        }
        output << '\n';
    }
    
    output.close();
}
