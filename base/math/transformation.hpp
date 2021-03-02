#pragma once

#include "./vector3.hpp"
#include "./vector4.hpp"
#include "./matrix3x3.hpp"
#include "./matrix4x4.hpp"

template <typename T> FUNC_QUALIFIER
Vector4<T> point_cast(const Vector3<T>& v);

template <typename T> FUNC_QUALIFIER
Vector3<T> point_cast(const Vector4<T>& v);

template <typename T> FUNC_QUALIFIER
Vector4<T> vec_cast(const Vector3<T>& v);

template <typename T> FUNC_QUALIFIER
Vector3<T> vec_cast(const Vector4<T>& v);

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Matrix3x3<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> translate(T x, T y, T z);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> translate(const Vector3<T>& v);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> rotate(const Vector3<T>& axis, T angle);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> scale(T sx, T sy, T sz);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> scale(const Vector3<T>& s);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> lookAt(
	const Vector3<T>& position, 
	const Vector3<T>& target, 
	const Vector3<T>& up);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> perspective(T fovy, T aspect, T zNear, T zFar);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> frustum(T left, T right, T bottom, T top, T zNear, T zFar);

/******************* implementation *******************/

template <typename T> FUNC_QUALIFIER
Vector3<T> point_cast(const Vector4<T>& v) {
	const T rw = static_cast<T>(1) / v.w;
	return { rw * v.x, rw * v.y, rw * v.z };
}

template <typename T> FUNC_QUALIFIER
Vector4<T> point_cast(const Vector3<T>& v) {
	return { v.x, v.y, v.z, static_cast<T>(1) };
}

template <typename T> FUNC_QUALIFIER
Vector4<T> vec_cast(const Vector3<T>& v) {
	return { v.x, v.y, v.z, static_cast<T>(0) };
}

template <typename T> FUNC_QUALIFIER
Vector3<T> vec_cast(const Vector4<T>& v) {
	return { v.x, v.y, v.z };
}

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Matrix4x4<T>& m) {
	return Matrix3x3<T> {
		m.e00, m.e10, m.e20,
		m.e01, m.e11, m.e21,
		m.e02, m.e12, m.e22
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Matrix3x3<T>& m) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	return Matrix4x4<T> {
		m.e00, m.e10, m.e20, zero,
		m.e01, m.e11, m.e21, zero,
		m.e02, m.e12, m.e22, zero,
		 zero,  zero,  zero,  one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> translate(T x, T y, T z) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	return Matrix4x4<T> {
		 one, zero, zero, zero,
		zero,  one, zero, zero,
		zero, zero,  one, zero,
		   x,    y,    z,  one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> translate(const Vector3<T>& v) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	return Matrix4x4<T> {
		 one, zero, zero, zero,
		zero,  one, zero, zero,
		zero, zero,  one, zero,
		 v.x,  v.y,  v.z,  one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> rotate(const Vector3<T>& axis, T angle) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	const T s = sin(angle);
	const T c = cos(angle);
	const T C = 1 - c;

	return Matrix4x4<T>{
		c + axis.x * axis.x * C,          // m00
		axis.x* axis.y* C + axis.z * s,   // m10
		axis.x* axis.z* C - axis.y * s,   // m20
		zero,                             // m30
		axis.x* axis.y* C - axis.z * s,   // m01
		c + axis.y * axis.y * C,          // m11
		axis.y* axis.z* C + axis.x * s,   // m21
		zero,                             // m31
		axis.x* axis.z* C + axis.y * s,   // m02
		axis.y* axis.z* C - axis.x * s,   // m12
		c + axis.z * axis.z * C,          // m22
		zero,                             // m32
		zero,                             // m03
		zero,                             // m13
		zero,                             // m23
		one                               // m33
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> scale(T sx, T sy, T sz) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	return Matrix4x4<T> {
		  sx, zero, zero, zero,
		zero,   sy, zero, zero,
		zero, zero,   sz, zero,
		zero, zero, zero,  one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> scale(const Vector3<T>& s) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	return Matrix4x4<T> {
		 s.x, zero, zero, zero,
		zero,  s.y, zero, zero,
		zero, zero,  s.z, zero,
		zero, zero, zero,  one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> lookAt(
	const Vector3<T>& position,
	const Vector3<T>& target,
	const Vector3<T>& up) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	const Vector3<T> zAxis = normalize(position - target);
	const Vector3<T> xAxis = normalize(cross(up, zAxis));
	const Vector3<T> yAxis = cross(zAxis, xAxis);

	return Matrix4x4<T> {
		              xAxis.x,               yAxis.x,               zAxis.x, zero,
		              xAxis.y,               yAxis.y,               zAxis.y, zero,
		              xAxis.z,               yAxis.z,               zAxis.z, zero,
		-dot(position, xAxis), -dot(position, yAxis), -dot(position, zAxis),  one
	};
}


template <typename T> FUNC_QUALIFIER
Matrix4x4<T> ortho(T left, T right, T bottom, T top, T zNear, T zFar) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);
	
	const T rRightMinusLeft = one / (right - left);
	const T rTopMinusBottom = one / (top - bottom);
	const T rNearMinusFar = one / (zNear - zFar);

	return Matrix4x4<T> {
		two * rRightMinusLeft, zero, zero, zero,
		zero, two * rTopMinusBottom, zero, zero,
		zero, zero, two * rNearMinusFar, zero,
		-(right + left) * rRightMinusLeft, \
		-(top + bottom) * rTopMinusBottom, \
		(zNear + zFar) * rNearMinusFar, one
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> perspective(T fovy, T aspect, T zNear, T zFar) {
	constexpr T zero = static_cast<T>(0);
	constexpr T oneHalf = static_cast<T>(0.5);
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);

	 const T rt = one / tan(oneHalf * fovy);
	 const T rdz = one / (zNear - zFar);

	return Matrix4x4<T> {
		rt / aspect, zero,                     zero, zero,
		zero,          rt,                     zero, zero,
		zero,        zero,     (zNear + zFar) * rdz, -one,
		zero,        zero, two * zNear * zFar * rdz, zero
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> frustum(T left, T right, T bottom, T top, T zNear, T zFar) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);

	const T rRightMinusLeft = one / (right - left);
	const T rTopMinusBottom = one / (top - bottom);
	const T rNearMinusFar = one / (zNear - zFar);

	return Matrix4x4<T> {
		two * zNear * rRightMinusLeft, zero, zero, zero,
		zero, two * zNear * rTopMinusBottom, zero, zero,
		(right + left) * rRightMinusLeft, \
		(top + bottom) * rTopMinusBottom, \
		(zNear + zFar) * rNearMinusFar, -one,
		zero, zero, two * zNear * zFar * rNearMinusFar, zero
	};
}
