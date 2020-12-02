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





void Scene::create_object(const std::string& line, const Eigen::Matrix4d& transformationMatrix, double cutoffAngle) {
    std::shared_ptr<Object> newObject = std::make_shared<Object>(line, transformationMatrix, cutoffAngle);
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




Ray Scene::determine_pixelray(int pixh, int pixw) {
    Ray ray;
    
    double py = pixh / (mImageHeight - 1) * (mCamera.mBottomBound - mCamera.mTopBound) + mCamera.mTopBound;
    double px = pixw / (mImageWidth - 1) * (mCamera.mRightBound - mCamera.mLeftBound) + mCamera.mLeftBound;
    
    ray.mPosition = mCamera.mEyePosition + (mCamera.mNearClippingPlane * mCamera.mWDirection) + (px * mCamera.mUDirection) + (py * mCamera.mVDirection);
    
    ray.mDirection = ray.mPosition - mCamera.mEyePosition;
    ray.mDirection = ray.mDirection / ray.mDirection.norm();
    
    return ray;
}





Eigen::Vector3d Scene::illuminate_point(const Ray& ray, const std::shared_ptr<Object>& pBestObject) {
    Eigen::Vector3d color;
    
    return color;
}




void Scene::raytrace(Ray& ray, Eigen::Vector3d& accumulation, Eigen::Vector3d& reflectance, int depth) {
    Eigen::Vector3d color(0.0, 0.0, 0.0);
    
    std::shared_ptr<Object> pBestObject = nullptr;
    Face bestFace;
    Eigen::Vector3d bestPoint;
    Eigen::Vector3d surfaceNormal;
    bool hit = false;
    double bestT = DBL_MAX;
    double bestBeta, bestGamma;
    
    for (auto& pObject : mpObjects) {
        pObject->ray_intersect(ray, pBestObject, bestFace, bestBeta, bestGamma, bestT);
        
        if (pBestObject != nullptr) {
            bestPoint = ray.mPosition + bestT * ray.mDirection;
            hit = true;
        }
    }
    
    if (hit) {
        Eigen::Vector3d matAmbient;
        Eigen::Vector3d matDiffuse;
        Eigen::Vector3d matSpecular;
        Eigen::Vector3d matReflect;
        double specularExponent;
        double refractionIndex = 0;
        int illumination;
        
        std::shared_ptr<Sphere> pSphere = std::dynamic_pointer_cast<Sphere>(pBestObject);
        if (pSphere) {
            surfaceNormal = bestPoint - pSphere->mPosition;
            surfaceNormal = surfaceNormal / surfaceNormal.norm();
            
            matAmbient = pSphere->mAmbientReflection;
            matDiffuse = pSphere->mDiffuseReflection;
            matSpecular = pSphere->mSpecularReflection;
            matReflect = pSphere->mAttenuationReflection;
            specularExponent = 16;
            refractionIndex = pSphere->mRefractionIndex;
            illumination = 3;
        }
        else {
            surfaceNormal = bestFace.calculate_point_normal(bestBeta, bestGamma);
            matAmbient = bestFace.mMaterial->mAmbient;
            matDiffuse = bestFace.mMaterial->mDiffuse;
            matSpecular = bestFace.mMaterial->mSpecular;
            specularExponent = bestFace.mMaterial->mSpecularExponent;
            illumination = bestFace.mMaterial->mIllumination;
            
            if (illumination == 2) {
                matReflect << 0.0, 0.0, 0.0;
            }
            else if (illumination == 3) {
                matReflect = matSpecular;
            }
        }
        
        color(0) = mAmbientLight.mColor(0) * matAmbient(0);
        color(1) = mAmbientLight.mColor(1) * matAmbient(1);
        color(2) = mAmbientLight.mColor(2) * matAmbient(2);
        
        Eigen::Vector3d pointToLight, pointToRay, spr;
        double surfaceProjection, cdr;
        
        
        Ray rayToLight;
        double blockingT;
        std::shared_ptr<Object> pBlockingObject;
        Face blockingFace;
        bool blocked = false;
        
        pointToRay = ray.mPosition - bestPoint;
        pointToRay = pointToRay / pointToRay.norm();
        
        // Hack for two faces
        if (pointToRay.dot(surfaceNormal) < 0.0) {
            surfaceNormal = -1 * surfaceNormal;
        }
        
        for (const PointLight& light : mPointLights) {
            pBlockingObject = nullptr;
            blocked = false;
            
            pointToLight = light.mPosition - bestPoint;
            blockingT = pointToLight.norm();
            pointToLight = pointToLight / pointToLight.norm();
            
            rayToLight.mPosition = bestPoint;
            rayToLight.mDirection = pointToLight;
             
            for (auto& pObject : mpObjects) {
                if (pObject != pBestObject) {
                    pObject->ray_intersect(rayToLight, pBlockingObject, bestFace, bestBeta, bestGamma, blockingT);
                    if (pBlockingObject != nullptr) {
                        blocked = true;
                        break;
                    }
                }
            }
            
            if (!blocked) {
                
                surfaceProjection = surfaceNormal.dot(pointToLight);
                
                if (surfaceProjection > 0.0) {
                    
                    color(0) += light.mColor(0) * matDiffuse(0) * surfaceProjection;
                    color(1) += light.mColor(1) * matDiffuse(1) * surfaceProjection;
                    color(2) += light.mColor(2) * matDiffuse(2) * surfaceProjection;
                    
                    spr = (2 * surfaceProjection * surfaceNormal) - pointToLight;
                    spr = spr / spr.norm();
                    
                    cdr = pointToRay.dot(spr);
                    
                    if (cdr > 0.0) {
                        color(0) += light.mColor(0) * matSpecular(0) * (pow(cdr, specularExponent));
                        color(1) += light.mColor(1) * matSpecular(1) * (pow(cdr, specularExponent));
                        color(2) += light.mColor(2) * matSpecular(2) * (pow(cdr, specularExponent));
                    }
                }
            }
        }
        
        color(0) *= reflectance(0);
        color(1) *= reflectance(1);
        color(2) *= reflectance(2);
        
//        color(0) *= reflectance(0) * matSpecular(0);
//        color(1) *= reflectance(1) * matSpecular(1);
//        color(2) *= reflectance(2) * matSpecular(2);
        
//        accumulation += color;
        
        if (depth > 0) {
            Ray reflectionRay;
            reflectionRay.mPosition = bestPoint;
            
            Eigen::Vector3d inverseRayDirection = -1 * ray.mDirection;
            reflectionRay.mDirection = 2 * surfaceNormal.dot(inverseRayDirection) * surfaceNormal - inverseRayDirection;
            
            Eigen::Vector3d reflectionAccumulation(0.0, 0.0, 0.0);
            
            Eigen::Vector3d newReflectance = reflectance;
            newReflectance(0) *= matReflect(0);
            newReflectance(1) *= matReflect(1);
            newReflectance(2) *= matReflect(2);
            raytrace(reflectionRay, reflectionAccumulation, newReflectance, depth-1);
            
            accumulation(0) += reflectance(0) * matSpecular(0) * reflectionAccumulation(0);
            accumulation(1) += reflectance(1) * matSpecular(1) * reflectionAccumulation(1);
            accumulation(2) += reflectance(2) * matSpecular(2) * reflectionAccumulation(2);
        }
        
        if (pSphere && depth > 0 && matSpecular.sum() < 3.0) {
            Eigen::Vector3d inverseRayDirection = -1 * ray.mDirection;
            bool success;
            Ray refractionRay = pSphere->calculate_refraction_exit_ray(inverseRayDirection, surfaceNormal, bestPoint, refractionIndex, 1.0, success);
            
            if (success) {
                color(0) *= matSpecular(0);
                color(1) *= matSpecular(1);
                color(2) *= matSpecular(2);
                
                Eigen::Vector3d refractionAccumulation(0.0, 0.0, 0.0);
                
                Eigen::Vector3d newReflectance = reflectance;
                newReflectance(0) *= (1 - matReflect(0));
                newReflectance(1) *= (1 - matReflect(1));
                newReflectance(2) *= (1 - matReflect(2));
                raytrace(refractionRay, refractionAccumulation, newReflectance, depth-1);
                
                accumulation(0) += reflectance(0) * (1.0 - matSpecular(0)) * refractionAccumulation(0);
                accumulation(1) += reflectance(1) * (1.0 - matSpecular(1)) * refractionAccumulation(1);
                accumulation(2) += reflectance(2) * (1.0 - matSpecular(2)) * refractionAccumulation(2);
            }
        }
        
        accumulation += color;
    }
}





void Scene::render(const std::string& imageName) {
    std::ofstream output(imageName);
    Eigen::Vector3d pixelColors;
    Ray ray;
    
    output << "P3\n";
    output << mImageWidth << ' ' << mImageHeight << ' ' << "255\n";
    for (int i = 0; i < mImageHeight; i++) {
        for (int j = 0; j < mImageWidth; j++) {
            ray = determine_pixelray(i, j);
            Eigen::Vector3d pixelColors(0.0, 0.0, 0.0);
            Eigen::Vector3d reflectance(1.0, 1.0, 1.0);
            raytrace(ray, pixelColors, reflectance, mRecursionLevel);
            
            pixelColors = convert_to_255(pixelColors);
            output << pixelColors(0) << ' ' << pixelColors(1) << ' ' << pixelColors(2);
            
            if (j < mImageWidth - 1) {
                output << ' ';
            }
            else {
                output << '\n';
            }
        }
    }
    
    output.close();
}
