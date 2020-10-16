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
    Eigen::Vector3d mEyePos;
    Eigen::Vector3d mLookAt;
    Eigen::Vector3d mUpVector;
    
    int mNearClippingPlane;
    
    int mLeftBound;
    int mRightBound;
    int mBottomBound;
    int mTopBound;
};
typedef struct Camera_t Camera;



struct AmbientLight_t {
    Eigen::Vector3d mColorLevels;
};
typedef struct AmbientLight_t AmbientLight;



struct PointLight_t {
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mColorLevels;
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
    
    void create_resolution(const std::string& line);
    void create_camera(const std::string& line);
    void update_bounds(const std::string& line);
    void create_ambient_light(const std::string& line);
    void create_point_light(const std::string& line);
    void create_object(const std::string& line);
    
    void output_image();
};

#endif //SCENE_H
