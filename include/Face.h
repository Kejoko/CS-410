// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef FACE_H
#define FACE_H

#include <memory>
#include <vector>

#include <Eigen/Dense>

#include "Material.h"
#include "Ray.h"
#include "Vertex.h"

class Object;

class Face {
public:
    Face();
    
    int mIndex;
    std::vector<int> mVertexIndices;
    std::vector<Eigen::Vector3d> mAverageVertexNormals;
    Eigen::Vector3d mNormal;
    std::shared_ptr<Material> mMaterial;
    
    Object* mpObject;
    
    void ray_intersect(const Ray&, bool& hit, double& t, double& beta, double& gamma);
    void calculate_average_vertex_normals();
    Eigen::Vector3d calculate_point_normal(double beta, double gamma);
};;

#endif //FACE_H
