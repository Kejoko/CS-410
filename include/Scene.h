// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "Object.h"
#include "Sphere.h"

struct Camera_t {
    Eigen::Vector3d mEyePosition;
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



struct PixelRay_t {
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mDirection;
};
typedef struct PixelRay_t PixelRay;



class Scene {
public:
    Scene() = default;
    Scene(const std::string& outImageName);
    
    int mImageWidth;
    int mImageHeight;
    
    Camera mCamera;
    AmbientLight mAmbientLight;
    std::vector<PointLight> mPointLights;
    std::vector<std::shared_ptr<Object>> mpObjects;
    
    void create_resolution(const std::string& line);
    void create_camera(const std::string& line);
    void update_bounds(const std::string& line);
    void create_ambient_light(const std::string& line);
    void create_point_light(const std::string& line);
    void create_sphere(const std::string& line);
    void create_object(const std::string& line);
    
    PixelRay determine_pixelray(int pixw, int pixh);
    Eigen::Vector3d determine_pixel_colors(int pixw, int pixh);
    void output_image(const std::string& imageName);
};

#endif //SCENE_H
