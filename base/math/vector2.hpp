#pragma once

#include "math_util.hpp"


namespace gtm {
template <typename T>
struct Vector2 {
public:
	union {
		struct { T x, y; };
		struct { T u, v; };
		T data[2];
	};

public:
	FUNC_QUALIFIER Vector2();
	FUNC_QUALIFIER Vector2(T x, T y);
	FUNC_QUALIFIER Vector2(const Vector2<T>& v);
	FUNC_QUALIFIER ~Vector2();

	FUNC_QUALIFIER Vector2& operator=(const Vector2& v);

	FUNC_QUALIFIER T& operator[](const int i);
	FUNC_QUALIFIER const T& operator[](const int i) const;

	FUNC_QUALIFIER Vector2& operator+=(const Vector2& v);
	FUNC_QUALIFIER Vector2& operator-=(const Vector2& v);
	FUNC_QUALIFIER Vector2& operator*=(const T c);
	FUNC_QUALIFIER Vector2& operator/=(const T c);

	FUNC_QUALIFIER T length() const;
	FUNC_QUALIFIER void normalize();
	FUNC_QUALIFIER T dot(const Vector2& v) const;
};

template <typename T> FUNC_QUALIFIER
Vector2<T> operator+(const Vector2<T>& v);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator-(const Vector2<T>& v);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator*(const T c, const Vector2<T>& v);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator*(const Vector2<T>& v, const T c);

template <typename T> FUNC_QUALIFIER
Vector2<T> operator/(const Vector2<T>& v, const T c);

template <typename T> FUNC_QUALIFIER
Vector2<T> normalize(const Vector2<T>& v);

template <typename T> FUNC_QUALIFIER
T dot(const Vector2<T>& u, const Vector2<T>& v);
} // namespace gtm


namespace gtm {
//--------------- class function implementation ---------------//
template <typename T> FUNC_QUALIFIER
	Vector2<T>::Vector2() : x(0), y(0) { }

template <typename T> FUNC_QUALIFIER
	Vector2<T>::Vector2(T x, T y) : x(x), y(y) { }

template <typename T> FUNC_QUALIFIER
	Vector2<T>::Vector2(const Vector2<T>& v) : x(v.x), y(v.y) { }

template <typename T> FUNC_QUALIFIER
	Vector2<T>::~Vector2() { }

template <typename T> FUNC_QUALIFIER
	Vector2<T>& Vector2<T>::operator=(const Vector2<T>& v) {
	x = v.x, y = v.y;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	T& Vector2<T>::operator[](const int i) {
	return data[i];
}

template <typename T> FUNC_QUALIFIER
	const T& Vector2<T>::operator[](const int i) const {
	return data[i];
}

template <typename T> FUNC_QUALIFIER
	Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& v) {
	x += v.x, y += v.y;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& v) {
	x -= v.x, y -= v.y;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	Vector2<T>& Vector2<T>::operator*=(const T c) {
	x *= c, y *= c;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	Vector2<T>& Vector2<T>::operator/=(const T c) {
	const T rc = static_cast<T>(1) / c;
	x *= rc, y *= rc;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	T Vector2<T>::length() const {
	return sqrt(x * x + y * y);
}

template <typename T> FUNC_QUALIFIER
	void Vector2<T>::normalize() {
	const T rn = static_cast<T>(1) / sqrt(x * x + y * y);
	x *= rn, y *= rn, z *= rn;
}

template <typename T> FUNC_QUALIFIER
	T Vector2<T>::dot(const Vector2<T>& v) const {
	return x * v.x + y * v.y;
}


//--------------- global function implementation ---------------//
template <typename T> FUNC_QUALIFIER
	Vector2<T> operator+(const Vector2<T>& v) {
	return v;
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator-(const Vector2<T>& v) {
	return Vector2<T>(-v.x, -v.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs) {
	return Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs) {
	return Vector2<T>(lhs.x - rhs.x, lhs.y - rhs.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator*(const T c, const Vector2<T>& v) {
	return Vector2<T>(c * v.x, c * v.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator*(const Vector2<T>& v, const T c) {
	return Vector2<T>(c * v.x, c * v.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> operator/(const Vector2<T>& v, const T c) {
	T rc = static_cast<T>(1) / c;
	return Vector2<T>(rc * v.x, rc * v.y);
}

template <typename T> FUNC_QUALIFIER
	Vector2<T> normalize(const Vector2<T>& v) {
	const T rn = static_cast<T>(1) / sqrt(v.x * v.x + v.y * v.y);
	return Vector2<T>(v.x * rn, v.y * rn);
}

template <typename T> FUNC_QUALIFIER
	T dot(const Vector2<T>& u, const Vector2<T>& v) {
	return u.x * v.x + u.y * v.y;
}

} // namespace gtm