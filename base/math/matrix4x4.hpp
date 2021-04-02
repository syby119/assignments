#pragma once

#include <utility>

#include "./math_util.hpp"
#include "./vector4.hpp"

namespace gtm {
template <typename T>
struct Matrix4x4 {
public:
	using column_type = Vector4<T>;

public:
	union {
		struct {
			T e00, e10, e20, e30;
			T e01, e11, e21, e31;
			T e02, e12, e22, e32;
			T e03, e13, e23, e33;
		};
		Vector4<T> cols[4];
	};

public:
	FUNC_QUALIFIER Matrix4x4();

	FUNC_QUALIFIER Matrix4x4(const T c);

	FUNC_QUALIFIER Matrix4x4(
		T x0, T y0, T z0, T w0, 
		T x1, T y1, T z1, T w1,
		T x2, T y2, T z2, T w2,
		T x3, T y3, T z3, T w3);

	FUNC_QUALIFIER Matrix4x4(const Matrix4x4& m);

	FUNC_QUALIFIER Matrix4x4(
		const Vector4<T>& v0,
		const Vector4<T>& v1,
		const Vector4<T>& v2,
		const Vector4<T>& v3);

	FUNC_QUALIFIER ~Matrix4x4();

	FUNC_QUALIFIER Matrix4x4& operator=(const Matrix4x4& m);

	FUNC_QUALIFIER column_type& operator[](const int i);
	FUNC_QUALIFIER const column_type& operator[](const int i) const;

	FUNC_QUALIFIER Matrix4x4& operator+=(const Matrix4x4& m);
	FUNC_QUALIFIER Matrix4x4& operator-=(const Matrix4x4& m);
	FUNC_QUALIFIER Matrix4x4& operator*=(const T c);
	FUNC_QUALIFIER Matrix4x4& operator*=(const Matrix4x4& m);
	FUNC_QUALIFIER Matrix4x4& operator/=(const T c);
	FUNC_QUALIFIER Matrix4x4& operator/=(const Matrix4x4& m);

	FUNC_QUALIFIER T determinant() const;
	FUNC_QUALIFIER void transpose();
	FUNC_QUALIFIER void inverse();
};

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator+(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator-(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator+(const Matrix4x4<T>& m, const Matrix4x4<T>& n);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator-(const Matrix4x4<T>& m, const Matrix4x4<T>& n);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const T c, const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const Matrix4x4<T>& m, const T c);

template <typename T> FUNC_QUALIFIER
Vector4<T> operator*(const Matrix4x4<T>& m, const Vector4<T>& v);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const Matrix4x4<T>& m, const Matrix4x4<T>& n);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const Matrix4x4<T>& m, const T c);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const T c, const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const Matrix4x4<T>& m, const Matrix4x4<T>& n);

template <typename T> FUNC_QUALIFIER
T determinant(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> transpose(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> inverse(const Matrix4x4<T>& m);
} // namespace gtm



/*********************** implementation ***********************/
namespace gtm {
//-------------- class function implementation --------------//
template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::Matrix4x4() : cols{ } { }

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::Matrix4x4(const T c)
	: cols{ { c, 0, 0, 0 },
			{ 0, c, 0, 0 },
			{ 0, 0, c, 0 },
			{ 0, 0, 0, c } } { }

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::Matrix4x4(
	T x0, T y0, T z0, T w0,
	T x1, T y1, T z1, T w1,
	T x2, T y2, T z2, T w2,
	T x3, T y3, T z3, T w3)
	: cols{ { x0, y0, z0, w0 }, 
			{ x1, y1, z1, w1 }, 
			{ x2, y2, z2, w2 }, 
			{ x3, y3, z3, w3 } } { }

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::Matrix4x4(
	const Vector4<T>& v0,
	const Vector4<T>& v1,
	const Vector4<T>& v2,
	const Vector4<T>& v3)
	: cols{ v0, v1, v2, v3 } {}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::Matrix4x4(const Matrix4x4& m) 
	: cols{ m.cols[0], m.cols[1], m.cols[2], m.cols[3] } { }

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>::~Matrix4x4() { }

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator=(const Matrix4x4<T>& m) {
	cols[0] = m.cols[0];
	cols[1] = m.cols[1];
	cols[2] = m.cols[2];
	cols[3] = m.cols[3];

	return *this;
}

template <typename T> FUNC_QUALIFIER
Vector4<T>& Matrix4x4<T>::operator[](const int i) {
	return cols[i];
}

template <typename T> FUNC_QUALIFIER
const Vector4<T>& Matrix4x4<T>::operator[](const int i) const {
	return cols[i];
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator+=(const Matrix4x4<T>& m) {
	cols[0] += m.cols[0];
	cols[1] += m.cols[1];
	cols[2] += m.cols[2];
	cols[3] += m.cols[3];

	return *this;
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator-=(const Matrix4x4<T>& m) {
	cols[0] -= m.cols[0];
	cols[1] -= m.cols[1];
	cols[2] -= m.cols[2];
	cols[3] -= m.cols[3];

	return *this;
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator*=(const T c) {
	cols[0] *= c;
	cols[1] *= c;
	cols[2] *= c;
	cols[3] *= c;

	return *this;
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator*=(const Matrix4x4<T>& m) {
	return (*this = (*this) * m);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator/=(const T c) {
	T rc = static_cast<T>(1) / c;
	cols[0] *= rc;
	cols[1] *= rc;
	cols[2] *= rc;
	cols[3] *= rc;

	return *this;
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T>& Matrix4x4<T>::operator/=(const Matrix4x4<T>& m) {
	return (*this *= ::inverse(m));
}

template <typename T> FUNC_QUALIFIER
T Matrix4x4<T>::determinant() const {
	T sub2_01_01 = e22 * e33 - e32 * e23;
	T sub2_01_02 = e21 * e33 - e23 * e31;
	T sub2_01_03 = e21 * e32 - e22 * e31;
	T sub2_01_12 = e20 * e33 - e23 * e30;
	T sub2_01_13 = e20 * e32 - e22 * e30;
	T sub2_01_23 = e20 * e31 - e21 * e30;

	T sub3_0_0 = e11 * sub2_01_01 - e12 * sub2_01_02 + e13 * sub2_01_03;
	T sub3_0_1 = e10 * sub2_01_01 - e12 * sub2_01_12 + e13 * sub2_01_13;
	T sub3_0_2 = e10 * sub2_01_02 - e11 * sub2_01_12 + e13 * sub2_01_23;
	T sub3_0_3 = e10 * sub2_01_03 - e11 * sub2_01_13 + e12 * sub2_01_23;

	return e00 * sub3_0_0 - e01 * sub3_0_1 + e02 * sub3_0_2 - e03 * sub3_0_3;
}

template <typename T> FUNC_QUALIFIER
void Matrix4x4<T>::transpose() {
	std::swap(e01, e10);
	std::swap(e02, e20);
	std::swap(e03, e30);
	std::swap(e12, e21);
	std::swap(e13, e31);
	std::swap(e23, e32);
}

template <typename T> FUNC_QUALIFIER
void Matrix4x4<T>::inverse() {
	*this = ::inverse(*this);
}

//-------------- global function implementation --------------//
template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator+(const Matrix4x4<T>& m) {
	return m;
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator-(const Matrix4x4<T>& m) {
	return Matrix4x4<T>(-m[0], -m[1], -m[2], -m[3]);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator+(const Matrix4x4<T>& m, const Matrix4x4<T>& n) {
	return Matrix4x4<T>(m[0] + n[0], m[1] + n[1], m[2] + n[2], m[3] + n[3]);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator-(const Matrix4x4<T>& m, const Matrix4x4<T>& n) {
	return Matrix4x4<T>(m[0] - n[0], m[1] - n[1], m[2] - n[2], m[3] - n[3]);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const T c, const Matrix4x4<T>& m) {
	return Matrix4x4<T>(c * m[0], c * m[1], c * m[2], c * m[3]);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const Matrix4x4<T>& m, const T c) {
	return Matrix4x4<T>(c * m[0], c * m[1], c * m[2], c * m[3]);
}

template <typename T> FUNC_QUALIFIER
Vector4<T> operator*(const Matrix4x4<T>& m, const Vector4<T>& v) {
	return Vector4<T>(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z + m.e03 * v.w,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z + m.e13 * v.w,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z + m.e23 * v.w,
		m.e30 * v.x + m.e31 * v.y + m.e32 * v.z + m.e33 * v.w);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator*(const Matrix4x4<T>& m, const Matrix4x4<T>& n) {
	return Matrix4x4<T>(
		m.e00 * n.e00 + m.e01 * n.e10 + m.e02 * n.e20 + m.e03 * n.e30, // m00
		m.e10 * n.e00 + m.e11 * n.e10 + m.e12 * n.e20 + m.e13 * n.e30, // m10
		m.e20 * n.e00 + m.e21 * n.e10 + m.e22 * n.e20 + m.e23 * n.e30, // m20
		m.e30 * n.e00 + m.e31 * n.e10 + m.e32 * n.e20 + m.e33 * n.e30, // m30
		m.e00 * n.e01 + m.e01 * n.e11 + m.e02 * n.e21 + m.e03 * n.e31, // m01
		m.e10 * n.e01 + m.e11 * n.e11 + m.e12 * n.e21 + m.e13 * n.e31, // m11
		m.e20 * n.e01 + m.e21 * n.e11 + m.e22 * n.e21 + m.e23 * n.e31, // m21
		m.e30 * n.e01 + m.e31 * n.e11 + m.e32 * n.e21 + m.e33 * n.e31, // m31
		m.e00 * n.e02 + m.e01 * n.e12 + m.e02 * n.e22 + m.e03 * n.e32, // m02
		m.e10 * n.e02 + m.e11 * n.e12 + m.e12 * n.e22 + m.e13 * n.e32, // m12
		m.e20 * n.e02 + m.e21 * n.e12 + m.e22 * n.e22 + m.e23 * n.e32, // m22
		m.e30 * n.e02 + m.e31 * n.e12 + m.e32 * n.e22 + m.e33 * n.e32, // m32
		m.e00 * n.e03 + m.e01 * n.e13 + m.e02 * n.e23 + m.e03 * n.e33, // m03
		m.e10 * n.e03 + m.e11 * n.e13 + m.e12 * n.e23 + m.e13 * n.e33, // m13
		m.e20 * n.e03 + m.e21 * n.e13 + m.e22 * n.e23 + m.e23 * n.e33, // m23
		m.e30 * n.e03 + m.e31 * n.e13 + m.e32 * n.e23 + m.e33 * n.e33);// m33
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const Matrix4x4<T>& m, const T c) {
	T rc = static_cast<T>(1) / c;
	return Matrix4x4<T>(rc * m[0], rc * m[1], rc * m[2], rc * m[3]);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const T c, const Matrix4x4<T>& m) {
	return c * inverse(m);
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> operator/(const Matrix4x4<T>& m, const Matrix4x4<T>& n) {
	return m * inverse(n);
}

template <typename T> FUNC_QUALIFIER
T determinant(const Matrix4x4<T>& m) {
	return m.determinant();
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> transpose(const Matrix4x4<T>& m) {
	return Matrix4x4<T>(
		m.e00, m.e01, m.e02, m.e03,
		m.e10, m.e11, m.e12, m.e13,
		m.e20, m.e21, m.e22, m.e23,
		m.e30, m.e31, m.e32, m.e33);
}

/*
 * @detail the 2x2 block form of matrix4x4 is 
 *			M = |A B|
 *			    |C D|
 *		   inverse(M) = |(|D|A - B(D#C))#    (|B|C-D(A#B)#)# |
 *                      |(|C|B - A((D#C)#))# (|A|D-C(A#B))#  | / determinant(M)
 *		   determinant(M) = |A||D| + |B||C| - tr((A#B)(D#C))
 *		   tr(AB) = A00B00 + A01B10 + A10B01 + A11B11
 *		   A# = | A11 -A01|
 *              |-A10  A00|
 */
template <typename T> FUNC_QUALIFIER
Matrix4x4<T> inverse(const Matrix4x4<T>& m) {
	T ab00 = m.e11 * m.e02 - m.e01 * m.e12;
	T ab01 = m.e11 * m.e03 - m.e01 * m.e13;
	T ab10 = m.e00 * m.e12 - m.e10 * m.e02;
	T ab11 = m.e00 * m.e13 - m.e10 * m.e03;

	T dc00 = m.e20 * m.e33 - m.e23 * m.e30;
	T dc01 = m.e21 * m.e33 - m.e23 * m.e31;
	T dc10 = m.e22 * m.e30 - m.e20 * m.e32;
	T dc11 = m.e22 * m.e31 - m.e21 * m.e32;

	T adet = m.e00 * m.e11 - m.e10 * m.e01;
	T bdet = m.e02 * m.e13 - m.e12 * m.e03;
	T cdet = m.e20 * m.e31 - m.e30 * m.e21;
	T ddet = m.e22 * m.e33 - m.e32 * m.e23;

	T rdet = static_cast<T>(1) / 
		(adet * ddet + bdet * cdet
		- ab00 * dc00 - ab01 * dc10 - ab10 * dc01 - ab11 * dc11);

	return Matrix4x4<T>(
		rdet * (ddet * m.e11 - m.e12 * dc01 - m.e13 * dc11), // m00
		rdet * (m.e12 * dc00 + m.e13 * dc10 - ddet * m.e10), // m10
		rdet * (cdet * m.e13 + dc01 * m.e10 - dc00 * m.e11), // m20
		rdet * (dc11 * m.e10 - dc10 * m.e11 - cdet * m.e12), // m30
		rdet * (m.e02 * dc01 + m.e03 * dc11 - ddet * m.e01), // m01
		rdet * (ddet * m.e00 - m.e02 * dc00 - m.e03 * dc10), // m11
		rdet * (dc00 * m.e01 - dc01 * m.e00 - cdet * m.e03), // m21
		rdet * (cdet * m.e02 - m.e00 * dc11 + m.e01 * dc10), // m31
		rdet * (bdet * m.e31 - ab00 * m.e33 + ab01 * m.e32), // m02
		rdet * (ab11 * m.e32 - ab10 * m.e33 - bdet * m.e30), // m12
		rdet * (adet * m.e33 - ab01 * m.e30 - ab11 * m.e31), // m22
		rdet * (ab00 * m.e30 + ab10 * m.e31 - adet * m.e32), // m32
		rdet * (ab00 * m.e23 - ab01 * m.e22 - bdet * m.e21), // m03
		rdet * (bdet * m.e20 + ab10 * m.e23 - ab11 * m.e22), // m13
		rdet * (ab01 * m.e20 + ab11 * m.e21 - adet * m.e23), // m23
		rdet * (adet * m.e22 - ab00 * m.e20 - ab10 * m.e21)  // m33
	);
}

} // namespace gtm