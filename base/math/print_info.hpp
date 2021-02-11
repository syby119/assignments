#pragma once

#include <cstdio>

#include "./quaternion.hpp"
#include "./vector3.hpp"
#include "./vector4.hpp"
#include "./matrix3x3.hpp"
#include "./matrix4x4.hpp"

template <typename T> FUNC_DECL
void print(const Quaternion<T>& q) {
    printf("(%f, %f, %f, %f)\n", q.w, q.x, q.y, q.z);
}

template <typename T> FUNC_DECL
void print(const Vector3<T>& v) {
    printf("(%f, %f, %f)\n", v.x, v.y, v.z);
}

template <typename T> FUNC_DECL
void print(const Vector4<T>& v) {
    printf("(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

template <typename T> FUNC_DECL
void print(const Matrix3x3<T>& m) {
    printf("%f %f %f\n", m[0][0], m[1][0], m[2][0]);
    printf("%f %f %f\n", m[0][1], m[1][1], m[2][1]);
    printf("%f %f %f\n", m[0][2], m[1][2], m[2][2]);
}

template <typename T> FUNC_DECL
void print(const Matrix4x4<T>& m) {
    printf("%f %f %f %f\n", m[0][0], m[1][0], m[2][0], m[3][0]);
    printf("%f %f %f %f\n", m[0][1], m[1][1], m[2][1], m[3][1]);
    printf("%f %f %f %f\n", m[0][2], m[1][2], m[2][2], m[3][2]);
    printf("%f %f %f %f\n", m[0][3], m[1][3], m[2][3], m[3][3]);
}
