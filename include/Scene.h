// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>

#include <Eigen/Dense>

#include "Object.h"

struct Camera_t {
    Eigen::Vector4d mEyePos;
    Eigen::Vector4d mLookAt;
    Eigen::Vector4d mUpVector;
    
    int mNearClippingPlane;
    
    int mLeftBound;
    int mRightBound;
    int mBottomBound;
    int mTopBound;
};
typedef struct Camera_t Camera;



struct AmbientLight_t {
    Eigen::Vector4d mColorLevels;
};
typedef struct AmbientLight_t AmbientLight;



struct PointLight_t {
    Eigen::Vector4d mPosition;
    Eigen::Vector4d mColorLevels;
};
typedef struct PointLight_t PointLight;



class Scene {
public:
    Scene() = default;
    Scene(const std::string& outImageName);
    
    std::string mOutImageName;
    int mImageWidth;
    int mImageHeight;
    
    Camera mCamera;
    AmbientLight mAmbientLight;
    std::vector<PointLight> mPointLights;
    std::vector<Object> mObjects;
    
    void create_resolution();
    void create_camera();
    void update_bounds();
    void create_ambient_light();
    void create_point_light();
    void create_object();
    
    void output_image();
};

#endif //SCENE_H
