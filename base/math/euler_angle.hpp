#pragma once

#include "./math_util.hpp"

#if defined GTM_DEFAULT_ROTATE_ORDER_XYZ
#define GTM_DEFAULT_ROTATE_ORDER RotateOrder::XYZ
#elif defined GTM_DEFAULT_ROTATE_ORDER_XZY
#define GTM_DEDAULT_ROTATE_ORDER RotateOrder::XZY
#elif defined GTM_DEFAULT_ROTATE_ORDER_YXZ
#define GTM_DEDAULT_ROTATE_ORDER RotateOrder::YXZ
#elif defined GTM_DEFAULT_ROTATE_ORDER_YZX
#define GTM_DEDAULT_ROTATE_ORDER RotateOrder::YZX
#elif defined GTM_DEFAULT_ROTATE_ORDER_ZXY
#define GTM_DEDAULT_ROTATE_ORDER RotateOrder::ZXY
#elif defined GTM_DEFAULT_ROTATE_ORDER_ZYX
#define GTM_DEDAULT_ROTATE_ORDER RotateOrder::ZYX
#endif

#ifndef GTM_DEFAULT_ROTATE_ORDER
#define GTM_DEFAULT_ROTATE_ORDER RotateOrder::XYZ
#endif

namespace gtm {
enum class RotateOrder {
	XYZ, XZY, YXZ, YZX, ZXY, ZYX
};

template <typename T>
struct EulerAngle {
public:
	union {
		struct { T x, y, z; };
		struct { T alpha, beta, gamma; };
		struct { T phi, theta, psi; };
		struct { T roll, pitch, yaw; };
	};

	RotateOrder order{ GTM_DEFAULT_ROTATE_ORDER };

public:
	FUNC_QUALIFIER EulerAngle();
	FUNC_QUALIFIER EulerAngle(
		T alpha, T beta, T gamma, enum RotateOrder order = GTM_DEFAULT_ROTATE_ORDER);
	FUNC_QUALIFIER EulerAngle(const EulerAngle& e);
	FUNC_QUALIFIER ~EulerAngle();

	FUNC_QUALIFIER EulerAngle& operator=(const EulerAngle& e);
	FUNC_QUALIFIER EulerAngle& operator+=(const EulerAngle& e);
	FUNC_QUALIFIER EulerAngle& operator-=(const EulerAngle& e);
	FUNC_QUALIFIER EulerAngle& operator*=(const T c);
	FUNC_QUALIFIER EulerAngle& operator/=(const T c);
};

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator+(const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator-(const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator+(const EulerAngle<T>& lhs, const EulerAngle<T>& rhs);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator-(const EulerAngle<T>& lhs, const EulerAngle<T>& rhs);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator*(const T c, const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator*(const EulerAngle<T>& e, const T c);

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator/(const EulerAngle<T>& e, const T c);
} // namespace gtm



/********************* implementation *********************/
namespace gtm {
//------------- class function implementation -------------//
template <typename T> FUNC_QUALIFIER
	EulerAngle<T>::EulerAngle() : alpha(0), beta(0), gamma(0) { }

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>::EulerAngle(T alpha, T beta, T gamma, enum RotateOrder order)
	: alpha(alpha), beta(beta), gamma(gamma), order(order) { }

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>::EulerAngle(const EulerAngle<T>& e)
	: alpha(e.alpha), beta(e.beta), gamma(e.gamma), order(e.order) { }

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>::~EulerAngle() { }

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>& EulerAngle<T>::operator=(const EulerAngle<T>& e) {
	alpha = e.alpha, beta = e.beta, gamma = e.gamma, order = e.order;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>& EulerAngle<T>::operator+=(const EulerAngle<T>& e) {
	assert(order == e.order);
	alpha += e.alpha, beta += e.beta, gamma += e.gamma;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>& EulerAngle<T>::operator-=(const EulerAngle<T>& e) {
	assert(order == e.order);
	alpha -= e.alpha, beta -= e.beta, gamma -= e.gamma;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>& EulerAngle<T>::operator*=(const T c) {
	alpha *= c, beta *= c, gamma *= c;
	return *this;
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T>& EulerAngle<T>::operator/=(const T c) {
	const T rc = static_cast<T>(1) / c;
	alpha *= rc, beta *= rc, gamma *= rc;
	return *this;
}

//------------- global function implementation -------------//
template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator+(const EulerAngle<T>& e) {
	return e;
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator-(const EulerAngle<T>& e) {
	return { -e.x, -e.y, -e.z, e.order };
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator+(const EulerAngle<T>& lhs, const EulerAngle<T>& rhs) {
	assert(lhs.order == rhs.order);
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.order };
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator-(const EulerAngle<T>& lhs, const EulerAngle<T>& rhs) {
	assert(lhs.order == rhs.order);
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.order };
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator*(const T c, const EulerAngle<T>& e) {
	return { c * e.x, c * e.y, c * e.z, e.order };
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator*(const EulerAngle<T>& e, const T c) {
	return { c * e.x, c * e.y, c * e.z, e.order };
}

template <typename T> FUNC_QUALIFIER
	EulerAngle<T> operator/(const EulerAngle<T>& e, const T c) {
	const T rc = static_cast<T>(1) / c;
	return { rc * e.x, rc * e.y, rc * e.z, e.order };
}

} // namespace gtm