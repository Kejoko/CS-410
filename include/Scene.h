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
#include "Ray.h"
#include "Sphere.h"

struct Camera_t {
    Eigen::Vector3d mEyePosition;
    Eigen::Vector3d mLookAt;
    Eigen::Vector3d mUpVector;
    Eigen::Vector3d mUDirection;
    Eigen::Vector3d mVDirection;
    Eigen::Vector3d mWDirection;
    
    double mNearClippingPlane;
    
    double mLeftBound;
    double mRightBound;
    double mBottomBound;
    double mTopBound;
};
typedef struct Camera_t Camera;



struct AmbientLight_t {
    Eigen::Vector3d mColor;
};
typedef struct AmbientLight_t AmbientLight;



struct PointLight_t {
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mColor;
};
typedef struct PointLight_t PointLight;



class Scene {
public:
    Scene() = default;
    Scene(const std::string& outImageName);
    
    double mImageWidth;
    double mImageHeight;
    
    int mRecursionLevel;
    
    Camera mCamera;
    AmbientLight mAmbientLight;
    std::vector<PointLight> mPointLights;
    std::vector<std::shared_ptr<Object>> mpObjects;
    
    void update_recursion_level(const std::string& line);
    void create_resolution(const std::string& line);
    void create_camera(const std::string& line);
    void update_bounds(const std::string& line);
    void create_ambient_light(const std::string& line);
    void create_point_light(const std::string& line);
    void create_sphere(const std::string& line);
    void create_object(const std::string& line, const Eigen::Matrix4d& transformationMatrix, double cutoffAngle);
    
    Ray determine_pixelray(int pixw, int pixh);
    void raytrace(Ray& ray, Eigen::Vector3d& accumulation, Eigen::Vector3d& reflectance, int depth);
    void render(const std::string& imageName);
};

#endif //SCENE_H
