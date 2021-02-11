#pragma once

#include "./math_util.hpp"

template <typename T>
struct Quaternion {
public:
    T w, x, y, z;

public: 
    FUNC_DECL Quaternion();
    FUNC_DECL Quaternion(T w, T x, T y, T z);
    FUNC_DECL Quaternion(const Quaternion& q);

    FUNC_DECL ~Quaternion();

    Quaternion& operator=(const Quaternion& q);

    FUNC_DECL Quaternion& operator+=(const Quaternion& q);
    FUNC_DECL Quaternion& operator-=(const Quaternion& q);
    FUNC_DECL Quaternion& operator*=(const T c);
    FUNC_DECL Quaternion& operator*=(const Quaternion& q);
    FUNC_DECL Quaternion& operator/=(const T c);
};

template <typename T> FUNC_DECL
Quaternion<T> operator+(const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator-(const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator+(const Quaternion<T>& p, const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator-(const Quaternion<T>& p, const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator*(const Quaternion<T>& p, const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator*(const Quaternion<T>& q, const T c);

template <typename T> FUNC_DECL
Quaternion<T> operator*(const T c, const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> operator/(const Quaternion<T>& q, const T c);

template <typename T> FUNC_DECL
T norm(const Quaternion<T>& q);

template <typename T> FUNC_DECL
T norm2(const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> conjugate(const Quaternion<T>& q);

template <typename T> FUNC_DECL
Quaternion<T> inverse(const Quaternion<T>& q);

/********************* class function implementation *********************/
template <typename T> FUNC_DECL
Quaternion<T>::Quaternion() : w(1), x(0), y(0), z(0) { }

template <typename T> FUNC_DECL
Quaternion<T>::Quaternion(T w, T x, T y, T z) : w(w), x(x), y(y), z(z) { }

template <typename T> FUNC_DECL
Quaternion<T>::Quaternion(const Quaternion& q) : w(q.w), x(q.x), y(q.y), z(q.z) { }

template <typename T> FUNC_DECL
Quaternion<T>::~Quaternion() { }

template <typename T>
Quaternion<T>& FUNC_DECL Quaternion<T>::operator=(const Quaternion& q) {
    w = q.w, x = q.x, y = q.y, z = q.z;
    return *this;
}

template <typename T> FUNC_DECL
Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& q) {
    w += q.w, x += q.x, y += q.y, z += q.z;
    return *this;
}

template <typename T> FUNC_DECL
Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& q) {
    w -= q.w, x -= q.x, y -= q.y, z -= q.z;
    return *this;
}

template <typename T> FUNC_DECL
Quaternion<T>& Quaternion<T>::operator*=(const T c) {
    w *= c, x *= c, y *= c, z *= c;
    return *this;
}

template <typename T> FUNC_DECL
Quaternion<T>& FUNC_DECL Quaternion<T>::operator*=(const Quaternion<T>& q) {
    const Quaternion p(*this);
    w = p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z;
    x = p.x * q.w + p.w * q.x - p.z * q.y + p.y * q.z;
    y = p.y * q.w + p.z * q.x + p.w * q.y - p.x * q.z;
    z = p.z * q.w - p.y * q.x + p.x * q.y + p.w * q.z;

    return *this;
}

template <typename T> FUNC_DECL
Quaternion<T>& Quaternion<T>::operator/=(const T c) {
    w /= c, x /= c, y /= c, z /= c;
    return *this;
}

/********************* global function implementation *********************/
template <typename T> FUNC_DECL
Quaternion<T> operator+(const Quaternion<T>& q) {
    return q;
}

template <typename T> FUNC_DECL
Quaternion<T> operator-(const Quaternion<T>& q) {
    return Quaternion<T>(-q.w, -q.x, -q.y, -q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> operator+(const Quaternion<T>& p, const Quaternion<T>& q) {
    return Quaternion<T>(p.w + q.w, p.x + q.x, p.y + q.y, p.z + q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> operator-(const Quaternion<T>& p, const Quaternion<T>& q) {
    return Quaternion<T>(p.w - q.w, p.x - q.x, p.y - q.y, p.z - q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> FUNC_DECL operator*(const T c, const Quaternion<T>& q) {
    return Quaternion<T>(c * q.w, c * q.x, c * q.y, c * q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> FUNC_DECL operator*(const Quaternion<T>& q, const T c) {
    return Quaternion<T>(c * q.w, c * q.x, c * q.y, c * q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> FUNC_DECL operator*(const Quaternion<T>& p, const Quaternion<T>& q) {
    return Quaternion<T>(
        p.w * q.w - p.x * q.x - p.y * q.y - p.z * q.z,
        p.x * q.w + p.w * q.x - p.z * q.y + p.y * q.z,
        p.y * q.w + p.z * q.x + p.w * q.y - p.x * q.z,
        p.z * q.w - p.y * q.x + p.x * q.y + p.w * q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> FUNC_DECL operator/(const Quaternion<T>& q, const T c) {
    return Quaternion<T>(q.w / c, q.x / c, q.y / c, q.z / c);
}

template <typename T> FUNC_DECL
T norm(const Quaternion<T>& q) {
    return sqrt(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}

template <typename T> FUNC_DECL
T norm2(const Quaternion<T>& q) {
    return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
}

template <typename T> FUNC_DECL
Quaternion<T> conjugate(const Quaternion<T>& q) {
    return Quaternion<T>(q.w, -q.x, -q.y, -q.z);
}

template <typename T> FUNC_DECL
Quaternion<T> inverse(const Quaternion<T>& q) {
    T rnorm = static_cast<T>(1) / (q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
    return Quaternion<T>(rnorm * q.w, -rnorm * q.x, -rnorm * q.y, -rnorm * q.z);
}
