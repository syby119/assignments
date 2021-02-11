/**
 * @file        base/math/3d_rotation.h
 * @brief       transform between different rotation method including 
 *              Euler angles, quaternion, rotation matrix and axis angles 
 * @detail      providing the transformation of the following:
 *                  Euler angles <-> quaternion
 *                  Euler angles <-> rotation matrix
 *                  quaternion <-> rotation matrix
 *                  axis angle -> quaternion
 *                  axis angle -> rotation matrix
 * @author      yy
 * @email       syby119@126.com
 * @date        2021/1/28
 * @copyright   MIT license
 */

#pragma once

#define GLM_FORCE_RADIANS
#ifdef GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


enum  class RotateOrder {
    XYZ, YZX, ZXY, XZY, YXZ, ZYX
};

enum  class RotateType {
    Intrinsic, Extrinsic
};

inline glm::quat eulerAnglesToQuaternion(const glm::vec3& eulerAngles, RotateOrder order, RotateType type) {

}

inline glm::vec3 quaternionToEulerAngles(const glm::quat& q) {

}

inline glm::mat4 quaternionToRotationMatrix(const glm::quaternion& q) {
    
}

inline glm::quat rotationMatrixToQuaternion(const glm::mat3& mat) {
    return 
}

inline glm::mat4 quaternionToRotationMatrix(const glm::quat& q) {
    return 
}

inline glm::mat4 eulerAnglesToRotationMatrix(const glm::vec3& v) {

}

inline glm::vec3 rotationMatrixToEulerAngles(const glm::mat4& m) {

}

inline glm::axisAngleToRotateMatrix(const glm::vec4& m) {

}

inline glm::mat4 axisAngleToRotateMatrix(const glm::vec3& axis, const angle) {

}

inline glm::mat4 axisAngleToQuaternion(const glm::vec3& axis, const angle) {

}