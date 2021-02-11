#pragma once

#include "./math_util.hpp"

template <typename T>
struct Vector4 {
public:
	union {
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		T data[4];
	};

public:
	FUNC_DECL Vector4();
	FUNC_DECL Vector4(T x, T y, T z, T w);
	FUNC_DECL Vector4(const Vector4<T>& v);
	FUNC_DECL ~Vector4();

	Vector4& operator=(const Vector4& v);

	T& operator[](const int i);
	const T& operator[](const int i) const;

	Vector4& operator+=(const Vector4& v);
	Vector4& operator-=(const Vector4& v);
	Vector4& operator*=(const T c);
	Vector4& operator/=(const T c);

	T dot(const Vector4& v);
};


template <typename T> FUNC_DECL
Vector4<T> operator+(const Vector4<T>& v);

template <typename T> FUNC_DECL
Vector4<T> operator-(const Vector4<T>& v);

template <typename T> FUNC_DECL
Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs);

template <typename T> FUNC_DECL
Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs);

template <typename T> FUNC_DECL
Vector4<T> operator*(const T c, const Vector4<T>& v);

template <typename T> FUNC_DECL
Vector4<T> operator*(const Vector4<T>& v, const T c);

template <typename T> FUNC_DECL
Vector4<T> operator/(const Vector4<T>& v, const T c);


/********************* class function implementation *********************/
template <typename T> FUNC_DECL
Vector4<T>::Vector4() : x(0), y(0), z(0), w(0) { }

template <typename T> FUNC_DECL
Vector4<T>::Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }

template <typename T> FUNC_DECL
Vector4<T>::Vector4(const Vector4<T>& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }

template <typename T> FUNC_DECL
Vector4<T>::~Vector4() { }

template <typename T> FUNC_DECL
Vector4<T>& Vector4<T>::operator=(const Vector4& v) {
	x = v.x, y = v.y, z = v.z, w = v.w;
	return *this;
}

template <typename T> FUNC_DECL
T& Vector4<T>::operator[](const int i) {
	return data[i];
}

template <typename T> FUNC_DECL
const T& Vector4<T>::operator[](const int i) const {
	return data[i];
}

template <typename T> FUNC_DECL
Vector4<T>& Vector4<T>::operator+=(const Vector4& v) {
	x += v.x, y += v.y, z += v.z, w += v.w;
	return *this;
}

template <typename T> FUNC_DECL
Vector4<T>& Vector4<T>::operator-=(const Vector4& v) {
	x -= v.x, y -= v.y, z -= v.z, w -= v.w;
	return *this;
}

template <typename T> FUNC_DECL
Vector4<T>& Vector4<T>::operator*=(const T c) {
	x *= c, y *= c, z *= c, w *= c;
	return *this;
}

template <typename T> FUNC_DECL
Vector4<T>& Vector4<T>::operator/=(const T c) {
	const T rc = static_cast<T>(1) / c;
	x *= rc, y *= rc, z *= rc, w *= rc;
	return *this;
}

template <typename T> FUNC_DECL
T Vector4<T>::dot(const Vector4<T>& v) {
	return x * v.x + y * v.y + z * v.z + w * v.w;
}

/********************* global function implementation *********************/
template <typename T> FUNC_DECL
Vector4<T> operator+(const Vector4<T>& v) {
	return v;
}

template <typename T> FUNC_DECL
Vector4<T> operator-(const Vector4<T>& v) {
	return Vector4<T>(-v.x, -v.y, -v.z, -v.w);
}

template <typename T> FUNC_DECL
Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs) {
	return Vector4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template <typename T> FUNC_DECL
Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs) {
	return Vector4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template <typename T> FUNC_DECL
Vector4<T> operator*(const T c, const Vector4<T>& v) {
	return Vector4<T>(c * v.x, c * v.y, c * v.z, c * v.w);
}

template <typename T> FUNC_DECL
Vector4<T> operator*(const Vector4<T>& v, const T c) {
	return Vector4<T>(c * v.x, c * v.y, c * v.z, c * v.w);
}

template <typename T> FUNC_DECL
Vector4<T> operator/(const Vector4<T>& v, const T c) {
	T rc = static_cast<T>(1) / c;
	return Vector4<T>(rc * v.x, rc * v.y, rc * v.z, rc * v.w);
}

template <typename T> FUNC_DECL
T dot(const Vector4<T>& u, const Vector4<T>& v) {
	return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}