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
#include "Sphere.h"

void Scene::create_resolution(const std::string& line) {
    std::istringstream iss(line);
    iss >> mImageWidth >> mImageHeight;
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
    
    std::shared_ptr<Sphere> sphere = std::static_pointer_cast<Sphere>(mpObjects.back());
}





void Scene::create_object(const std::string& line) {
    std::cout << line << '\n';
    
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




PixelRay Scene::determine_pixelray(int pixw, int pixh) {
    PixelRay ray;
    
    double xOffset = pixw / (mImageWidth - 1) * (mCamera.mRightBound - mCamera.mLeftBound) + mCamera.mLeftBound;
    double yOffset = pixh / (mImageHeight - 1) * (mCamera.mBottomBound - mCamera.mTopBound) + mCamera.mTopBound;
    
    ray.mPosition = mCamera.mEyePosition + (xOffset * mCamera.mUDirection) + (yOffset * mCamera.mVDirection) + (mCamera.mNearClippingPlane * mCamera.mWDirection);
    
    ray.mDirection = ray.mPosition - mCamera.mEyePosition;
    ray.mDirection = ray.mDirection / ray.mDirection.norm();
    
    return ray;
}





void Scene::shoot_ray(const PixelRay& ray, std::shared_ptr<Sphere>& bestSphere, Eigen::Vector3d& surfacePoint, int pixw, int pixh) {
    bestSphere = nullptr;
    
    double minDistance = DBL_MAX;
    double v, r2, c2, d, t;
    std::shared_ptr<Sphere> currentSphere;
    Eigen::Vector3d rayToCenter;
    
    for (int i = 0; i < mpObjects.size(); i++) {
        currentSphere = std::static_pointer_cast<Sphere>(mpObjects[i]);
        
        rayToCenter = currentSphere->mPosition - ray.mPosition;
        
        v = rayToCenter.dot(ray.mDirection);
        
        r2 = currentSphere->mRadius * currentSphere->mRadius;
        
        c2 = rayToCenter.dot(rayToCenter);
        
        d = r2 - (c2 - (v * v));
        
        if ((pixw >= 5 && pixw <= 8) && (pixh >= 5 && pixh <= 8) && (currentSphere->mId == 1 || currentSphere->mId == 2)) {
            std::cout << "considering: " << currentSphere->mId << "\td: " << d << '\n';
        }
        
        if (d > 0.0) {
            d = sqrt(d);
            if (d < minDistance) {
                bestSphere = currentSphere;
                minDistance = d;
                t = v - d;
                surfacePoint = ray.mPosition + t * ray.mDirection;
            }
        }
    }
}





Eigen::Vector3d Scene::color_sphere_point(const std::shared_ptr<Sphere>& sphere, const Eigen::Vector3d& surfacePoint) {
    Eigen::Vector3d surfaceNormal = surfacePoint - sphere->mPosition;
    surfaceNormal = surfaceNormal / surfaceNormal.norm();

    Eigen::Vector3d rgb;
    rgb(0) = mAmbientLight.mColor(0) * sphere->mAmbientReflection(0);
    rgb(1) = mAmbientLight.mColor(1) * sphere->mAmbientReflection(1);
    rgb(2) = mAmbientLight.mColor(2) * sphere->mAmbientReflection(2);
    
    Eigen::Vector3d surfacePointToLight;
    for (int i = 0; i < mPointLights.size(); i++) {
        surfacePointToLight = mPointLights[i].mPosition - surfacePoint;
        surfacePointToLight = surfacePointToLight / surfacePointToLight.norm();
        
        double strength = surfaceNormal.dot(surfacePointToLight);
        if (strength > 0.0) {
            rgb(0) += (mPointLights[i].mColor(0) * sphere->mDiffuseReflection(0)) * strength;
            rgb(1) += (mPointLights[i].mColor(1) * sphere->mDiffuseReflection(1)) * strength;
            rgb(2) += (mPointLights[i].mColor(2) * sphere->mDiffuseReflection(2)) * strength;
        }
    }
    
    return rgb;
}





Eigen::Vector3d Scene::determine_pixel_colors(int pixw, int pixh) {
    if ((pixw >= 5 && pixw <= 8) && (pixh >= 5 && pixh <= 8)) {
        std::cout << pixh << " , " << pixw << '\n';
    }
    
    PixelRay ray = determine_pixelray(pixw, pixh);
    
    std::shared_ptr<Sphere> sphere;
    Eigen::Vector3d surfacePoint;
    shoot_ray(ray, sphere, surfacePoint, pixw, pixh);
    
    Eigen::Vector3d rgb;
    if (sphere != nullptr) {
        rgb = color_sphere_point(sphere, surfacePoint);
        
        if ((pixw >= 5 && pixw <= 8) && (pixh >= 5 && pixh <= 8)) {
            std::cout << "Sphere: " << sphere->mId << '\n' << convert_to_255(rgb) << "\n\n";
        }
        
    }
    else {
        rgb(0) = 0;
        rgb(1) = 0;
        rgb(2) = 0;
    }
    
    return convert_to_255(rgb);
}





void Scene::output_image(const std::string& imageName) {
    std::ofstream output(imageName);
    
    output << "P3\n";
    output << mImageWidth << ' ' << mImageHeight << ' ' << "255\n";
    for (int i = 0; i < mImageHeight; i++) {
        for (int j = 0; j < mImageWidth; j++) {
            Eigen::Vector3d pixelColors = determine_pixel_colors(j, i);
            
            output << pixelColors(0) << ' ' << pixelColors(1) << ' ' << pixelColors(2);
            
            if (j < mImageHeight - 1)
                output << ' ';        }
        output << '\n';
    }
    
    output.close();
}
