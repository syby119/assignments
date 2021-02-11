#pragma once

#include <cstdint>
#include <utility>

#include "./math_util.hpp"
#include "./vector3.hpp"

template <typename T>
struct Matrix3x3 {
public:
	using column_type = Vector3<T>;

	FUNC_DECL Matrix3x3();
	
	FUNC_DECL Matrix3x3(const T c);

	FUNC_DECL Matrix3x3(
		T x0, T y0, T z0,
		T x1, T y1, T z1,
		T x2, T y2, T z2);
	
	FUNC_DECL Matrix3x3(const Matrix3x3& m);
	
	FUNC_DECL Matrix3x3(
		const column_type& column0,
		const column_type& column1,
		const column_type& column2);
	
	FUNC_DECL ~Matrix3x3();

	FUNC_DECL Matrix3x3& operator=(const Matrix3x3& m);
	
	FUNC_DECL column_type& operator[](const uint32_t i);
	FUNC_DECL const column_type& operator[](const uint32_t i) const;

	FUNC_DECL Matrix3x3& operator+=(const Matrix3x3& m);
	FUNC_DECL Matrix3x3& operator-=(const Matrix3x3& m);
	FUNC_DECL Matrix3x3& operator*=(const T c);
	FUNC_DECL Matrix3x3& operator*=(const Matrix3x3& m);
	FUNC_DECL Matrix3x3& operator/=(const T c);
	FUNC_DECL Matrix3x3& operator/=(const Matrix3x3& m);

	T determinant() const;
	void transpose();
	void inverse();

private:
	union {
		struct {
			T e00, e10, e20;
			T e01, e11, e21;
			T e02, e12, e22;
		};
		column_type cols[3];
	};
};

template <typename T> FUNC_DECL
Matrix3x3<T> operator+(const Matrix3x3<T>& m);

template <typename T> FUNC_DECL
Matrix3x3<T> operator-(const Matrix3x3<T>& m);

template <typename T> FUNC_DECL
Matrix3x3<T> operator+(const Matrix3x3<T>& m, const Matrix3x3<T>& n);

template <typename T> FUNC_DECL
Matrix3x3<T> operator-(const Matrix3x3<T>& m, const Matrix3x3<T>& n);

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const T c, const Matrix3x3<T>& m);

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const Matrix3x3<T>& m, const T c);

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const Matrix3x3<T>& m, const Matrix3x3<T>& n);

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const Matrix3x3<T>& m, const Matrix3x3<T>& n);

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const Matrix3x3<T>& m, const T c);

template <typename T> FUNC_DECL
Vector3<T> operator*(const Matrix3x3<T>& m, const Vector3<T>& v);

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const T c, const Matrix3x3<T>& m);

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const Matrix3x3<T>& m, const Matrix3x3<T>& n);

/********************* class function implementation *********************/
template <typename T> FUNC_DECL
Matrix3x3<T>::Matrix3x3(): cols{ } { }

template <typename T> FUNC_DECL
Matrix3x3<T>::Matrix3x3(const T c) 
	: cols{ { c, 0, 0 }, { 0, c, 0 }, { 0, 0, c } } { }


template <typename T> FUNC_DECL
Matrix3x3<T>::Matrix3x3(
	T x0, T y0, T z0,
	T x1, T y1, T z1,
	T x2, T y2, T z2)
	: cols{ { x0, y0, z0 }, { x1, y1, z1 }, { x2, y2, z2 } } { }

template <typename T> FUNC_DECL
Matrix3x3<T>::Matrix3x3(
	const column_type& column0,
	const column_type& column1,
	const column_type& column2)
	: cols{ column0, column1, column2 } { }

template <typename T> FUNC_DECL
Matrix3x3<T>::Matrix3x3(const Matrix3x3& m)
	: cols{ m.cols[0], m.cols[1], m.cols[2] } { }

template <typename T> FUNC_DECL
Matrix3x3<T>::~Matrix3x3() { }

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3& m) {
	cols[0] = m.cols[0];
	cols[1] = m.cols[1];
	cols[2] = m.cols[2];

	return *this;
}

template <typename T> FUNC_DECL
typename Matrix3x3<T>::column_type& Matrix3x3<T>::operator[](const uint32_t i) {
	return cols[i];
}

template <typename T> FUNC_DECL
const typename Matrix3x3<T>::column_type& Matrix3x3<T>::operator[](const uint32_t i) const {
	return cols[i];
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator+=(const Matrix3x3<T>& m) {
	cols[0] += m.cols[0];
	cols[1] += m.cols[1];
	cols[2] += m.cols[2];

	return *this;
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator-=(const Matrix3x3<T>& m) {
	cols[0] -= m.cols[0];
	cols[1] -= m.cols[1];
	cols[2] -= m.cols[2];

	return *this;
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator*=(const T c) {
	cols[0] *= c;
	cols[1] *= c;
	cols[2] *= c;

	return *this;
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator*=(const Matrix3x3& m) {
	return (*this = (*this * m));
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator/=(const T c) {
	const T rc = static_cast<T>(1) / c;
	cols[0] *= rc;
	cols[1] *= rc;
	cols[2] *= rc;

	return *this;
}

template <typename T> FUNC_DECL
Matrix3x3<T>& Matrix3x3<T>::operator/=(const Matrix3x3<T>& m) {
	 *this *= ::inverse(m);
	 return *this;
}

template <typename T> FUNC_DECL
T Matrix3x3<T>::determinant() const {
	return + e00 * (e11 * e22 - e21 * e12)
		   - e10 * (e01 * e22 - e21 * e02)
		   + e20 * (e01 * e12 - e11 * e02);
}

template <typename T> FUNC_DECL
void Matrix3x3<T>::transpose() {
	std::swap(e10, e01);
	std::swap(e20, e02);
	std::swap(e21, e12);
}

template <typename T> FUNC_DECL
void Matrix3x3<T>::inverse() {
	T rdet = static_cast<T>(1) / determinant();

	T d00 = rdet * (e11 * e22 - e21 * e12);
	T d01 = rdet * (e20 * e12 - e10 * e22);
	T d02 = rdet * (e10 * e21 - e20 * e11);
	T d10 = rdet * (e21 * e02 - e01 * e22);
	T d11 = rdet * (e00 * e22 - e20 * e02);
	T d12 = rdet * (e20 * e01 - e00 * e21);
	T d20 = rdet * (e01 * e12 - e11 * e02);
	T d21 = rdet * (e10 * e02 - e00 * e12);
	T d22 = rdet * (e00 * e11 - e10 * e01);

	e00 = d00, e10 = d01, e20 = d02;
	e01 = d10, e11 = d11, e21 = d12;
	e02 = d20, e12 = d21, e22 = d22;
}


/********************* global function implementation *********************/
template <typename T> FUNC_DECL
Matrix3x3<T> operator+(const Matrix3x3<T>& m) {
	return m;
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator-(const Matrix3x3<T>& m) {
	return Matrix3x3<T>(-m[0], -m[1], -m[2]);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator+(const Matrix3x3<T>& m, const Matrix3x3<T>& n) {
	return Matrix3x3<T>(m[0] + n[0], m[1] + n[1], m[2] + n[2]);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator-(const Matrix3x3<T>& m, const Matrix3x3<T>& n) {
	return Matrix3x3<T>(m[0] - n[0], m[1] - n[1], m[2] - n[2]);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const T c, const Matrix3x3<T>& m) {
	return Matrix3x3<T>(c * m[0], c * m[1], c * m[2]);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const Matrix3x3<T>& m, const T c) {
	return Matrix3x3<T>(c * m[0], c * m[1], c * m[2]);
}

template <typename T> FUNC_DECL
Vector3<T> operator*(const Matrix3x3<T>& m, const Vector3<T>& v) {
	return Vector3<T>(
		m.e00 * v.x + m.e01 * v.y + m.e02 * v.z,
		m.e10 * v.x + m.e11 * v.y + m.e12 * v.z,
		m.e20 * v.x + m.e21 * v.y + m.e22 * v.z);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator*(const Matrix3x3<T>& m, const Matrix3x3<T>& n) {
	return Matrix3x3<T>(
		m.e00 * n.e00 + m.e01 * n.e10 + m.e02 * n.e20, // m00
		m.e10 * n.e00 + m.e11 * n.e10 + m.e12 * n.e20, // m10
		m.e20 * n.e00 + m.e21 * n.e10 + m.e22 * n.e20, // m20
		m.e00 * n.e01 + m.e01 * n.e11 + m.e02 * n.e21, // m01
		m.e10 * n.e01 + m.e11 * n.e11 + m.e12 * n.e21, // m11
		m.e20 * n.e01 + m.e21 * n.e11 + m.e22 * n.e21, // m21
		m.e00 * n.e02 + m.e01 * n.e12 + m.e02 * n.e22, // m02
		m.e10 * n.e02 + m.e11 * n.e12 + m.e12 * n.e22, // m12
		m.e20 * n.e02 + m.e21 * n.e12 + m.e22 * n.e22);// m22
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const Matrix3x3<T>& m, const T c) {
	T rc = static_cast<T>(1) / c;
	return Matrix3x3<T>(rc * m[0], rc * m[1], rc * m[2]);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const T c, const Matrix3x3<T>& m) {
	return c * inverse(m);
}

template <typename T> FUNC_DECL
Matrix3x3<T> operator/(const Matrix3x3<T>& m, const Matrix3x3<T>& n) {
	return m * inverse(n);
}

template <typename T> FUNC_DECL
T determinant(const Matrix3x3<T>& m) {
	return m.determinant();
}

template <typename T> FUNC_DECL
Matrix3x3<T> transpose(const Matrix3x3<T>& m) {
	return Matrix3x3<T>(
		m.e00, m.e01, m.e02,
		m.e10, m.e11, m.e12,
		m.e20, m.e21, m.e22);
}

template <typename T> FUNC_DECL
Matrix3x3<T> inverse(const Matrix3x3<T>& m) {
	T rdet = static_cast<T>(1) / m.determinant();

	return Matrix3x3<T>(
		rdet * (m.e11 * m.e22 - m.e21 * m.e12), // m.e00
		rdet * (m.e20 * m.e12 - m.e10 * m.e22), // m.e10
		rdet * (m.e10 * m.e21 - m.e20 * m.e11), // m.e20
		rdet * (m.e21 * m.e02 - m.e01 * m.e22), // m.e10
		rdet * (m.e00 * m.e22 - m.e20 * m.e02), // m.e20
		rdet * (m.e20 * m.e01 - m.e00 * m.e21), // m.e11
		rdet * (m.e01 * m.e12 - m.e11 * m.e02), // m.e02
		rdet * (m.e10 * m.e02 - m.e00 * m.e12), // m.e12
		rdet * (m.e00 * m.e11 - m.e10 * m.e01)  // m.e22
	);
}
