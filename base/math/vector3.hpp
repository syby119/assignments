#pragma once

#include "./math_util.hpp"

template <typename T>
struct Vector3 {
public:
	union {
		struct { T x, y, z; };
		struct { T r, g, b; };
		T data[3];
	};

public:
	FUNC_DECL Vector3();
	FUNC_DECL Vector3(T x, T y, T z);
	FUNC_DECL Vector3(const Vector3<T>& v);
	FUNC_DECL ~Vector3();

	Vector3& operator=(const Vector3& v);

	T& operator[](const int i);
	const T& operator[](const int i) const;

	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(const T c);
	Vector3& operator/=(const T c);

	T dot(const Vector3& v);
	Vector3<T> cross(const Vector3& v);
};


template <typename T> FUNC_DECL
Vector3<T> operator+(const Vector3<T>& v);

template <typename T> FUNC_DECL
Vector3<T> operator-(const Vector3<T>& v);

template <typename T> FUNC_DECL
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T> FUNC_DECL
Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs);

template <typename T> FUNC_DECL
Vector3<T> operator*(const T c, const Vector3<T>& v);

template <typename T> FUNC_DECL
Vector3<T> operator*(const Vector3<T>& v, const T c);

template <typename T> FUNC_DECL
Vector3<T> operator/(const Vector3<T>& v, const T c);

template <typename T> FUNC_DECL
T dot(const Vector3<T>& u, const Vector3<T>& v);

template <typename T> FUNC_DECL
Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v);


/********************* class function implementation *********************/
template <typename T> FUNC_DECL
Vector3<T>::Vector3() : x(0), y(0), z(0) { }

template <typename T> FUNC_DECL
Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) { }

template <typename T> FUNC_DECL
Vector3<T>::Vector3(const Vector3<T>& v): x(v.x), y(v.y), z(v.z) { }

template <typename T> FUNC_DECL
Vector3<T>::~Vector3() { }

template <typename T> FUNC_DECL
Vector3<T>& Vector3<T>::operator=(const Vector3& v) {
	x = v.x, y = v.y, z = v.z;
	return *this;
}

template <typename T> FUNC_DECL
T& Vector3<T>::operator[](const int i) {
	return data[i];
}

template <typename T> FUNC_DECL
const T& Vector3<T>::operator[](const int i) const {
	return data[i];
}

template <typename T> FUNC_DECL
Vector3<T>& Vector3<T>::operator+=(const Vector3& v) {
	x += v.x, y += v.y, z += v.z;
	return *this;
}

template <typename T> FUNC_DECL
Vector3<T>& Vector3<T>::operator-=(const Vector3& v) {
	x -= v.x, y -= v.y, z -= v.z;
	return *this;
}

template <typename T> FUNC_DECL
Vector3<T>& Vector3<T>::operator*=(const T c) {
	x *= c, y *= c, z *= c;
	return *this;
}

template <typename T> FUNC_DECL
Vector3<T>& Vector3<T>::operator/=(const T c) {
	const T rc = static_cast<T>(1) / c;
	x *= rc, y *= rc, z *= rc;
	return *this;
}

template <typename T> FUNC_DECL
T Vector3<T>::dot(const Vector3<T>& v) {
	return x * v.x + y * v.y + z * v.z;
}

template <typename T> FUNC_DECL
Vector3<T> Vector3<T>::cross(const Vector3<T>& v) {
	return Vector3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

/********************* global function implementation *********************/
template <typename T> FUNC_DECL
Vector3<T> operator+(const Vector3<T>& v) {
	return v;
}

template <typename T> FUNC_DECL
Vector3<T> operator-(const Vector3<T>& v) {
	return Vector3<T>(-v.x, -v.y, -v.z);
}

template <typename T> FUNC_DECL
Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

template <typename T> FUNC_DECL
Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs) {
	return Vector3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

template <typename T> FUNC_DECL
Vector3<T> operator*(const T c, const Vector3<T>& v) {
	return Vector3<T>(c * v.x, c * v.y, c * v.z);
}

template <typename T> FUNC_DECL
Vector3<T> operator*(const Vector3<T>& v, const T c) {
	return Vector3<T>(c * v.x, c * v.y, c * v.z);
}

template <typename T> FUNC_DECL
Vector3<T> operator/(const Vector3<T>& v, const T c) {
	T rc = static_cast<T>(1) / c;
	return Vector3<T>(rc * v.x, rc * v.y, rc * v.z);
}

template <typename T> FUNC_DECL
T dot(const Vector3<T>& u, const Vector3<T>& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T> FUNC_DECL
Vector3<T> cross(const Vector3<T>& u, const Vector3<T>& v) {
	return Vector3<T>(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x);
}