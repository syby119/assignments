#pragma once

#include <cstdio>

#include "./vector3.hpp"
#include "./vector4.hpp"
#include "./quaternion.hpp"
#include "./euler_angle.hpp"
#include "./matrix3x3.hpp"
#include "./matrix4x4.hpp"


namespace gtm {
template <typename T> FUNC_QUALIFIER
void print(const Vector3<T>& v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

template <typename T> FUNC_QUALIFIER
void print(const Vector4<T>& v) {
    printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

template <typename T> FUNC_QUALIFIER
void print(const Quaternion<T>& q) {
    printf("(%f, %f, %f, %f)\n", q.w, q.x, q.y, q.z);
}

template <typename T> FUNC_QUALIFIER
void print(const EulerAngle<T>& e) {
    switch (e.order) {
    case RotateOrder::XYZ:
        printf("x-y-z intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    case RotateOrder::XZY:
        printf("x-z-y intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    case RotateOrder::YXZ:
        printf("y-x-z intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    case RotateOrder::YZX:
        printf("y-z-x intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    case RotateOrder::ZXY:
        printf("z-x-y intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    case RotateOrder::ZYX:
        printf("z-y-x intrinsic: (%f, %f, %f)\n", e.x, e.y, e.z);
        break;
    default:
        assert(false);
    }
}

template <typename T> FUNC_QUALIFIER
void print(const Matrix3x3<T>& m) {
    printf("%f %f %f\n", m[0][0], m[1][0], m[2][0]);
    printf("%f %f %f\n", m[0][1], m[1][1], m[2][1]);
    printf("%f %f %f\n", m[0][2], m[1][2], m[2][2]);
}



template <typename T> FUNC_QUALIFIER
void print(const Matrix4x4<T>& m) {
    printf("%f %f %f %f\n", m[0][0], m[1][0], m[2][0], m[3][0]);
    printf("%f %f %f %f\n", m[0][1], m[1][1], m[2][1], m[3][1]);
    printf("%f %f %f %f\n", m[0][2], m[1][2], m[2][2], m[3][2]);
    printf("%f %f %f %f\n", m[0][3], m[1][3], m[2][3], m[3][3]);
}

} // namespace gtm