// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef OBJECT_H
#define OBJECT_H

#include <array>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <Eigen/Dense>

#include "Face.h"
#include "Material.h"
#include "Ray.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    static int msObjectCount;
    int mObjectId;
    
    Object();
    Object(const std::string& fileName, const Eigen::Matrix4d& transformationMatrix, double cutoffAngle);
    
    std::string mFileName;
    double mCutoffAngle;
    std::vector<std::shared_ptr<Material>> mMaterialLibrary;
    std::shared_ptr<Material> mCurrentMaterial;
    std::vector<Eigen::Vector4d> mOldHomogeneousVertices;
    std::vector<Eigen::Vector4d> mHomogeneousVertices;
    std::vector<Eigen::Vector3d> mVertices;
    std::vector<Eigen::Vector4d> mVertexNormals;
    std::vector<Face> mFaces;
    std::string mSmoothing;
    std::vector<std::vector<int>> mLines;
    
    void handle_vertex(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_vertex_normal(const std::string& info, const Eigen::Matrix4d& transformationMatrix);
    void handle_smoothing(const std::string& info);
    void create_material_library(const std::string& fileName);
    void update_current_material(const std::string& materialName);
    void handle_face(const std::string& info);
    void handle_line(const std::string& info);
    
    virtual void ray_intersect(const Ray& ray, std::shared_ptr<Object>& pBestObject, Face& bestFace, double& bestT);
    
    double sum_absolute_translations();
    void output(const std::string& fileName);
};

#endif //OBJECT_H
