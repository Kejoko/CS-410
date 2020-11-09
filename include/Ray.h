// Keegan Kochis
// 831455801
// kkochis@rams.colostate.edu

#ifndef RAY_H
#define RAY_H

#include <Eigen/Dense>

struct Ray_t {
    Eigen::Vector3d mPosition;
    Eigen::Vector3d mDirection;
};
typedef struct Ray_t Ray;

#endif //RAY_H
