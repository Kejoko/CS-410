// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#include "Scene.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "Object.h"
#include "Sphere.h"

void Scene::create_resolution(const std::string& line) {
    std::istringstream iss(line);
    iss >> mImageWidth >> mImageHeight;
}





void Scene::create_camera(const std::string& line) {
    std::istringstream iss(line);
    int ex, ey, ez, lx, ly, lz, ux, uy, uz;
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
}





void Scene::update_bounds(const std::string& line) {
    std::istringstream iss(line);
    iss >> mCamera.mLeftBound >> mCamera.mRightBound >> mCamera.mBottomBound >> mCamera.mTopBound;
}





void Scene::create_ambient_light(const std::string& line) {
    std::istringstream iss(line);
    double r, g, b;
    iss >> r >> g >> b;
    
    mAmbientLight.mColorLevels(0) = r;
    mAmbientLight.mColorLevels(1) = g;
    mAmbientLight.mColorLevels(2) = b;
}





void Scene::create_point_light(const std::string& line) {
    std::istringstream iss(line);
    int x, y, z, w;
    double r, g, b;
    iss >> x >> y >> z >> w >> r >> g >> b;
    
    PointLight newLight;
    
    newLight.mPosition(0) = x;
    newLight.mPosition(1) = y;
    newLight.mPosition(2) = z;
    
    newLight.mColorLevels(0) = r;
    newLight.mColorLevels(1) = g;
    newLight.mColorLevels(2) = b;
    
    mPointLights.push_back(newLight);
}




void Scene::create_sphere(const std::string& line) {
    std::shared_ptr<Object> newSphere = std::make_shared<Sphere>(line);
    mpObjects.push_back(newSphere);
}





void Scene::create_object(const std::string& line) {
    std::cout << line << '\n';
    
}





Eigen::Vector3d convert_to_255(const Eigen::Vector3d& rgb) {
    Eigen::Vector3d rgbUpdated;
    
    rgbUpdated(0) = 255;
    rgbUpdated(1) = 255;
    rgbUpdated(2) = 255;
    
    return rgbUpdated;
}




PixelRay Scene::determine_pixelray(int pixw, int pixh) {
    PixelRay ray;
    
    ray.mPosition(0) = mCamera.mEyePosition(0) + mCamera.mLeftBound +
                       pixw * (mCamera.mRightBound - mCamera.mRightBound) / (mImageWidth - 1);
    ray.mPosition(1) = mCamera.mEyePosition(1) + mCamera.mBottomBound +
                       pixh * (mCamera.mTopBound - mCamera.mBottomBound) / (mImageHeight - 1);
    ray.mPosition(2) = mCamera.mEyePosition(2) + mCamera.mNearClippingPlane;
    
    ray.mDirection = ray.mPosition - mCamera.mEyePosition / (ray.mPosition - mCamera.mEyePosition).norm();
    
    return ray;
}





Eigen::Vector3d Scene::determine_pixel_colors(int pixw, int pixh) {
    PixelRay ray = determine_pixelray(pixw, pixh);
    
    
    
    
    Eigen::Vector3d rgb;
    
    rgb(0) = 0;
    rgb(1) = 0;
    rgb(2) = 0;
    
    return convert_to_255(rgb);
}

void Scene::output_image(const std::string& imageName) {
    std::ofstream output(imageName);
    
    output << "P3\n";
    output << mImageWidth << ' ' << mImageHeight << ' ' << "256\n";
    for (int i = 0; i < mImageWidth; i++) {
        for (int j = 0; j < mImageHeight; j++) {
            Eigen::Vector3d pixelColors = determine_pixel_colors(i, j);
            
            output << pixelColors(0) << ' ' << pixelColors(1) << ' ' << pixelColors(2);
            
            if (j < mImageHeight - 1)
                output << ' ';
        }
        output << '\n';
    }
    
    output.close();
}
