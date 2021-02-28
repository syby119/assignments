#pragma once

#include "./math_util.hpp"
#include "./matrix3x3.hpp"
#include "./matrix4x4.hpp"
#include "./quaternion.hpp"
#include "./euler_angle.hpp"

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Vector3<T>& axis, T angle);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Vector3<T>& axis, T angle);

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Quaternion<T>& q);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Quaternion<T>& q);

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Vector3<T>& axis, T angle);

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Matrix3x3<T>& m);

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Matrix4x4<T>& m);

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const EulerAngle<T>& e);

template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(
	const Matrix3x3<T>& m,
	enum RotateOrder order = DEFAULT_ROTATE_ORDER);

template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(
	const Matrix4x4<T>& m,
	enum RotateOrder order = DEFAULT_ROTATE_ORDER);

template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(
	const Quaternion<T>& q,
	enum RotateOrder order = DEFAULT_ROTATE_ORDER);


/****************** convert to matrix implementation ******************/
template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Vector3<T>& axis, T angle) {
	const T s = sin(angle);
	const T c = cos(angle);
	const T C = 1 - c;
	
	return Matrix3x3<T>{
		c + axis.x * axis.x * C,          // m00
		axis.x * axis.y * C + axis.z * s, // m10
		axis.x * axis.z * C - axis.y * s, // m20
		axis.x * axis.y * C - axis.z * s, // m01
		c + axis.y * axis.y * C,          // m11
		axis.y * axis.z * C + axis.x * s, // m21
		axis.x * axis.z * C + axis.y * s, // m02
		axis.y * axis.z * C - axis.x * s, // m12
		c + axis.z * axis.z * C           // m22
	};
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Vector3<T>& axis, T angle) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	const T s = sin(angle);
	const T c = cos(angle);
	const T C = 1 - c;

	return Matrix4x4<T>{
		c + axis.x * axis.x * C,          // m00
			axis.x* axis.y* C + axis.z * s, // m10
			axis.x* axis.z* C - axis.y * s, // m20
			zero,                             // m30
			axis.x* axis.y* C - axis.z * s, // m01
			c + axis.y * axis.y * C,          // m11
			axis.y* axis.z* C + axis.x * s, // m21
			zero,                             // m31
			axis.x* axis.z* C + axis.y * s, // m02
			axis.y* axis.z* C - axis.x * s, // m12
			c + axis.z * axis.z * C,          // m22
			zero,                             // m32
			zero,                             // m03
			zero,                             // m13
			zero,                             // m23
			one,                              // m33
	};
}

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const Quaternion<T>& q) {
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);

	const T ww = q.w * q.w;
	const T xx = q.x * q.x;
	const T yy = q.y * q.y;
	const T zz = q.z * q.z;

	const T wx = q.w * q.x;
	const T wy = q.w * q.y;
	const T wz = q.w * q.z;
	const T xy = q.x * q.y;
	const T xz = q.x * q.z;
	const T yz = q.y * q.z;

	return Matrix3x3<T>(
		one - two * (yy + zz), two * (xy + wz), two * (xz - wy),
		two * (xy - wz), one - two * (xx + zz), two * (yz + wx),
		two * (xz + wy), two * (yz - wx), one - two * (xx + yy));
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const Quaternion<T>& q) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);

	const T ww = q.w * q.w;
	const T xx = q.x * q.x;
	const T yy = q.y * q.y;
	const T zz = q.z * q.z;

	const T wx = q.w * q.x;
	const T wy = q.w * q.y;
	const T wz = q.w * q.z;
	const T xy = q.x * q.y;
	const T xz = q.x * q.z;
	const T yz = q.y * q.z;

	return Matrix4x4<T>(
		one - two * (yy + zz), two * (xy + wz), two * (xz - wy), zero,
		two * (xy - wz), one - two * (xx + zz), two * (yz + wx), zero,
		two * (xz + wy), two * (yz - wx), one - two * (xx + yy), zero,
		zero,            zero,            zero,                   one);
}

template <typename T> FUNC_QUALIFIER
Matrix3x3<T> mat3_cast(const EulerAngle<T>& e) {
	const T sx = sin(e.x), cx = cos(e.x);
	const T sy = sin(e.y), cy = cos(e.y);
	const T sz = sin(e.z), cz = cos(e.z);

	switch (e.order) {
	case RotateOrder::XYZ:
		return {
			 cy * cz, sx * sy * cz + sz * cx, sx * sz - sy * cx * cz,
			-sz * cy, cx * cz - sx * sy * sz, sx * cz + sy * sz * cx,
			      sy,               -sx * cy,                cx * cy
		};
	case RotateOrder::XZY:
		return {
			cy * cz, sx * sy + sz * cx * cy, sx * sz * cy - sy * cx,
				-sz,                cx * cz,                sx * cz,
			sy * cz, sy * sz * cx - sx * cy, sx * sy * sz + cx * cy
		};
	case RotateOrder::YXZ:
		return {
			sx * sy * sz + cy * cz, sz * cx, sx * sz * cy - sy * cz,
			sx * sy * cz - sz * cy, cx * cz, sx * cy * cz + sy * sz,
			               sy * cx,     -sx,                cx * cy
		};
	case RotateOrder::YZX:
		return {
			               cy * cz,       sz,               -sy * cz,
			sx * sy - sz * cx * cy,  cx * cz, sx * cy + sy * sz * cx,
			sx * sz * cy + sy * cx, -sx * cz, cx * cy - sx * sy * sz
		};
	case RotateOrder::ZXY:
		return {
			cy * cz - sx * sy * sz, sx * sy * cz + sz * cy, -sy * cx,
						  -sz * cx,                cx * cz,       sx,
			sx * sz * cy + sy * cz, sy * sz - sx * cy * cz,  cx * cy
		};
	case RotateOrder::ZYX:
		return {
			               cy * cz,                sz * cy,     -sy,
			sx * sy * cz - sz * cx, sx * sy * sz + cx * cz, sx * cy,
			sx * sz + sy * cx * cz, sy * sz * cx - sx * cz, cx * cy
		};
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}

template <typename T> FUNC_QUALIFIER
Matrix4x4<T> mat4_cast(const EulerAngle<T>& e) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);

	const T sx = sin(e.x), cx = cos(e.x);
	const T sy = sin(e.y), cy = cos(e.y);
	const T sz = sin(e.z), cz = cos(e.z);

	switch (e.order) {
	case RotateOrder::XYZ:
		return {
			 cy * cz, sx * sy * cz + sz * cx, sx * sz - sy * cx * cz, zero,
			-sz * cy, cx * cz - sx * sy * sz, sx * cz + sy * sz * cx, zero,
				  sy,               -sx * cy,                cx * cy, zero,
			    zero,                   zero,                   zero,  one
		};
	case RotateOrder::XZY:
		return {
			cy * cz, sx * sy + sz * cx * cy, sx * sz * cy - sy * cx, zero,
				-sz,                cx * cz,                sx * cz, zero,
			sy * cz, sy * sz * cx - sx * cy, sx * sy * sz + cx * cy, zero,
			   zero,                   zero,                   zero,  one
		};
	case RotateOrder::YXZ:
		return {
			sx * sy * sz + cy * cz, sz * cx, sx * sz * cy - sy * cz, zero,
			sx * sy * cz - sz * cy, cx * cz, sx * cy * cz + sy * sz, zero,
						   sy * cx,     -sx,                cx * cy, zero,
							  zero,    zero,                   zero,  one
		};
	case RotateOrder::YZX:
		return {
						   cy * cz,       sz,               -sy * cz, zero,
			sx * sy - sz * cx * cy,  cx * cz, sx * cy + sy * sz * cx, zero,
			sx * sz * cy + sy * cx, -sx * cz, cx * cy - sx * sy * sz, zero,
			                  zero,     zero,                   zero,  one
		};
	case RotateOrder::ZXY:
		return {
			cy * cz - sx * sy * sz, sx * sy * cz + sz * cy, -sy * cx, zero,
						  -sz * cx,                cx * cz,       sx, zero,
			sx * sz * cy + sy * cz, sy * sz - sx * cy * cz,  cx * cy, zero,
			                  zero,                   zero,     zero,  one
		};
	case RotateOrder::ZYX:
		return {
						   cy * cz,                sz * cy,     -sy, zero,
			sx * sy * cz - sz * cx, sx * sy * sz + cx * cz, sx * cy, zero,
			sx * sz + sy * cx * cz, sy * sz * cx - sx * cz, cx * cy, zero,
			                  zero,                   zero,    zero,  one
		};
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}


/****************** convert to quaternion implementation ******************/
template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Vector3<T>& axis, T angle) {
	angle *= static_cast<T>(0.5);
	const T s = sin(angle);

	return Quaternion<T>(cos(angle), s * axis.x, s * axis.y, s * axis.z);
}

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Matrix3x3<T>& m) {
	constexpr T one = static_cast<T>(1);
	constexpr T oneHalf = static_cast<T>(0.5);
	constexpr T oneFourth = static_cast<T>(0.25);

	const T fourW2Minus1 = m.e00 + m.e11 + m.e22;
	const T fourX2Minus1 = m.e00 - m.e11 - m.e22;
	const T fourY2Minus1 = m.e11 - m.e00 - m.e22;
	const T fourZ2Minus1 = m.e22 - m.e00 - m.e11;

	int condition = 0;
	T biggest = fourW2Minus1;
	if (biggest < fourX2Minus1) {
		condition = 1;
		biggest = fourX2Minus1;
	}
	if (biggest < fourY2Minus1) {
		condition = 2;
		biggest = fourY2Minus1;
	}
	if (biggest < fourZ2Minus1) {
		condition = 3;
		biggest = fourZ2Minus1;
	}

	biggest = oneHalf * sqrt(one + biggest);
	const T r = oneFourth / biggest;

	switch (condition) {
	case 0:
		return { biggest, r * (m.e21 - m.e12), r * (m.e02 - m.e20), r * (m.e10 - m.e01) };
	case 1:
		return { r * (m.e21 - m.e12), biggest, r * (m.e01 + m.e10), r * (m.e02 + m.e20) };
	case 2:
		return { r * (m.e02 - m.e20), r * (m.e01 + m.e10), biggest, r * (m.e12 + m.e21) };
	case 3:
		return { r * (m.e10 - m.e01), r * (m.e02 + m.e20), r * (m.e12 + m.e21), biggest };
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const Matrix4x4<T>& m) {
	constexpr T one = static_cast<T>(1);
	constexpr T oneHalf = static_cast<T>(0.5);
	constexpr T oneFourth = static_cast<T>(0.25);

	const T fourW2Minus1 = m.e00 + m.e11 + m.e22;
	const T fourX2Minus1 = m.e00 - m.e11 - m.e22;
	const T fourY2Minus1 = m.e11 - m.e00 - m.e22;
	const T fourZ2Minus1 = m.e22 - m.e00 - m.e11;

	int condition = 0;
	T biggest = fourW2Minus1;
	if (biggest < fourX2Minus1) {
		condition = 1;
		biggest = fourX2Minus1;
	}
	if (biggest < fourY2Minus1) {
		condition = 2;
		biggest = fourY2Minus1;
	}
	if (biggest < fourZ2Minus1) {
		condition = 3;
		biggest = fourZ2Minus1;
	}

	biggest = oneHalf * sqrt(one + biggest);
	const T r = oneFourth / biggest;

	switch (condition) {
	case 0:
		return { biggest, r * (m.e21 - m.e12), r * (m.e02 - m.e20), r * (m.e10 - m.e01) };
	case 1:
		return { r * (m.e21 - m.e12), biggest, r * (m.e01 + m.e10), r * (m.e02 + m.e20) };
	case 2:
		return { r * (m.e02 - m.e20), r * (m.e01 + m.e10), biggest, r * (m.e12 + m.e21) };
	case 3:
		return { r * (m.e10 - m.e01), r * (m.e02 + m.e20), r * (m.e12 + m.e21), biggest };
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}

template <typename T> FUNC_QUALIFIER
Quaternion<T> quat_cast(const EulerAngle<T>& e) {
	constexpr T halfOne = static_cast<T>(0.5);
	const T x = halfOne * e.x;
	const T y = halfOne * e.y;
	const T z = halfOne * e.z;

	const T sx = sin(x), cx = cos(x);
	const T sy = sin(y), cy = cos(y);
	const T sz = sin(z), cz = cos(z);
	
	switch (e.order) {
	case RotateOrder::XYZ:
		return {
			cx * cy * cz - sx * sy * sz,
			sx * cy * cz + cx * sy * sz,
			cx * sy * cz - sx * cy * sz,
			sx * sy * cz + cx * cy * sz };
	case RotateOrder::XZY:
		return {
			sx * sy * sz + cx * cy * cz,
			sx * cy * cz - cx * sy * sz,
			cx * sy * cz - sx * cy * sz,
			sx * sy * cz + cx * cy * sz };
	case RotateOrder::YXZ:
		return {
			sx * sy * sz + cx * cy * cz,
			sx * cy * cz + cx * sy * sz,
			cx * sy * cz - sx * cy * sz,
			cx * cy * sz - sx * sy * cz };
	case RotateOrder::YZX:
		return {
			cx * cy * cz - sx * sy * sz,
			sx * cy * cz + cx * sy * sz,
			sx * cy * sz + cx * sy * cz,
			cx * cy * sz - sx * sy * cz };
	case RotateOrder::ZXY:
		return {
			cx * cy * cz - sx * sy * sz,
			sx * cy * cz - cx * sy * sz,
			sx * cy * sz + cx * sy * cz,
			sx * sy * cz + cx * cy * sz };
	case RotateOrder::ZYX:
		return {
			sx * sy * sz + cx * cy * cz,
			sx * cy * cz - cx * sy * sz,
			sx * cy * sz + cx * sy * cz,
			cx * cy * sz - sx * sy * cz };
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}


/****************** convert to euler angle implementation ******************/
template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(const Matrix3x3<T>& m, enum RotateOrder order) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T pi_2 = static_cast<T>(0.5) * pi<T>();

	constexpr T upperBound = one - std::numeric_limits<T>::epsilon();
	constexpr T lowerBound = std::numeric_limits<T>::epsilon() - one;

	switch (order) {
	case RotateOrder::XYZ:
		if (m.e02 >= upperBound)
			return { atan2(m.e10, m.e11), pi_2, zero, order };
		else if (m.e02 <= lowerBound)
			return { atan2(m.e21, m.e11), -pi_2, zero, order };

		return { atan2(-m.e12, m.e22), asin(m.e02), atan2(-m.e01, m.e00), order };
	case RotateOrder::XZY:
		if (-m.e01 >= upperBound)
			return { atan2(m.e20, m.e10), zero, pi_2, order };
		else if (-m.e01 <= lowerBound)
			return { atan2(-m.e20, m.e22), zero, -pi_2, order };

		return { atan2(m.e21, m.e11), atan2(m.e02, m.e00), -asin(m.e01), order };
	case RotateOrder::YXZ:
		if (-m.e12 >= upperBound)
			return { pi_2, atan2(m.e01, m.e00), zero, order };
		else if (-m.e12 <= lowerBound)
			return { -pi_2, atan2(-m.e01, m.e00), zero, order };

		return { -asin(m.e12), atan2(m.e02, m.e22), atan2(m.e10, m.e11), order };
	case RotateOrder::YZX:
		if (m.e10 >= upperBound)
			return { atan2(m.e02, m.e22), zero, pi_2, order };
		else if (m.e10 <= lowerBound)
			return { atan2(m.e21, m.e01), zero, -pi_2, order };

		return { atan2(-m.e12, m.e11), atan2(-m.e20, m.e00), asin(m.e10), order };
	case RotateOrder::ZXY:
		if (m.e21 >= upperBound)
			return { pi_2, atan2(m.e02, m.e00), zero, order };
		else if (m.e21 <= lowerBound)
			return { -pi_2, atan2(m.e02, m.e00), zero, order };

		return { asin(m.e21), atan2(-m.e20, m.e22), atan2(-m.e01, m.e11), order };
	case RotateOrder::ZYX:
		if (-m.e20 >= upperBound)
			return { atan2(m.e01, m.e11), pi_2, zero, order };
		else if (-m.e20 <= lowerBound)
			return { atan2(-m.e01, m.e11), -pi_2, zero, order };

		return { atan2(m.e21, m.e22), -asin(m.e20), atan2(m.e10, m.e00), order };
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}

template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(const Matrix4x4<T>& m, enum RotateOrder order) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T pi_2 = static_cast<T>(0.5) * pi<T>();

	constexpr T upperBound = one - std::numeric_limits<T>::epsilon();
	constexpr T lowerBound = std::numeric_limits<T>::epsilon() - one;

	switch (order) {
	case RotateOrder::XYZ:
		if (m.e02 >= upperBound)
			return { atan2(m.e10, m.e11), pi_2, zero, order };
		else if (m.e02 <= lowerBound)
			return { atan2(m.e21, m.e11), -pi_2, zero, order };

		return { atan2(-m.e12, m.e22), asin(m.e02), atan2(-m.e01, m.e00), order };
	case RotateOrder::XZY:
		if (-m.e01 >= upperBound)
			return { atan2(m.e20, m.e10), zero, pi_2, order };
		else if (-m.e01 <= lowerBound)
			return { atan2(-m.e20, m.e22), zero, -pi_2, order };

		return { atan2(m.e21, m.e11), atan2(m.e02, m.e00), -asin(m.e01), order };
	case RotateOrder::YXZ:
		if (-m.e12 >= upperBound)
			return { pi_2, atan2(m.e01, m.e00), zero, order };
		else if (-m.e12 <= lowerBound)
			return { -pi_2, atan2(-m.e01, m.e00), zero, order };

		return { -asin(m.e12), atan2(m.e02, m.e22), atan2(m.e10, m.e11), order };
	case RotateOrder::YZX:
		if (m.e10 >= upperBound)
			return { atan2(m.e02, m.e22), zero, pi_2, order };
		else if (m.e10 <= lowerBound)
			return { atan2(m.e21, m.e01), zero, -pi_2, order };

		return { atan2(-m.e12, m.e11), atan2(-m.e20, m.e00), asin(m.e10), order };
	case RotateOrder::ZXY:
		if (m.e21 >= upperBound)
			return { pi_2, atan2(m.e02, m.e00), zero, order };
		else if (m.e21 <= lowerBound)
			return { -pi_2, atan2(m.e02, m.e00), zero, order };
	
		return { asin(m.e21), atan2(-m.e20, m.e22), atan2(-m.e01, m.e11), order };
	case RotateOrder::ZYX:
		if (-m.e20 >= upperBound)
			return { atan2(m.e01, m.e11), pi_2, zero, order };
		else if (-m.e20 <= lowerBound)
			return { atan2(-m.e01, m.e11), -pi_2, zero, order };
	
		return { atan2(m.e21, m.e22), -asin(m.e20), atan2(m.e10, m.e00), order };
	default:
		assert(false);
		return { }; // suppress warning C4715 in Visual Studio 
	}
}

template <typename T> FUNC_QUALIFIER
EulerAngle<T> euler_cast(const Quaternion<T>& q, enum RotateOrder order) {
	constexpr T zero = static_cast<T>(0);
	constexpr T one = static_cast<T>(1);
	constexpr T two = static_cast<T>(2);
	constexpr T pi_2 = static_cast<T>(0.5) * pi<T>();

	constexpr T upperBound = one - std::numeric_limits<T>::epsilon();
	constexpr T lowerBound = std::numeric_limits<T>::epsilon() - one;

	switch (order) {
		case RotateOrder::XYZ: {
			T e = two * (q.x * q.z + q.w * q.y);
			if (e >= upperBound) // q.y = q.w, q.z = q.x
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), pi_2, zero, order };
			else if (e <= lowerBound) // q.y = -q.w, q.z = -q.x
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), -pi_2, zero, order };

			return {
				atan2(two * (q.w * q.x - q.y * q.z), one - two * (q.x * q.x + q.y * q.y)),
				asin(e),
				atan2(two * (q.w * q.z - q.x * q.y), one - two * (q.y * q.y + q.z * q.z)),
				order
			};
		}
		case RotateOrder::XZY: {
			T e = two * (q.w * q.z - q.x * q.y);
			if (e >= upperBound) // q.y = -q.x, q.z = q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), zero, pi_2, order };
			else if (e <= lowerBound) // q.y = q.x, q.z = -q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), zero, -pi_2, order };

			return {
				atan2(two * (q.y * q.z + q.w * q.x), one - two * (q.x * q.x + q.z * q.z)),
				atan2(two * (q.x * q.z + q.w * q.y), one - two * (q.y * q.y + q.z * q.z)),
				asin(e),
				order 
			};
		}
		case RotateOrder::YXZ: {
			T e = two * (q.w * q.x - q.y * q.z);
			if (e >= upperBound) // q.x = q.w, q.z = -q.y
				return { pi_2, atan2(two * q.w * q.y, (q.w + q.y) * (q.w - q.y)), zero, order };
			else if (e <= lowerBound) // q.x = -q.w, q.z = q.y
				return { -pi_2, atan2(two * q.w * q.y, (q.w + q.y) * (q.w - q.y)), zero, order };

			return {
				asin(e),
				atan2(two * (q.x * q.z + q.w * q.y), one - two * (q.x * q.x + q.y * q.y)),
				atan2(two * (q.x * q.y + q.w * q.z), one - two * (q.x * q.x + q.z * q.z)),
				order
			};
		}
		case RotateOrder::YZX: {
			T e = two * (q.x * q.y + q.w * q.z);
			if (e >= upperBound) // q.x = q,y, q.z = q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), zero, pi_2, order };
			else if (e <= lowerBound) // q.x = -q.y, q.z = -q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), zero, -pi_2, order };
			
			return {
				atan2(two * (q.w * q.x - q.y * q.z), one - two * (q.x * q.x + q.z * q.z)),
				atan2(two * (q.w * q.y - q.x * q.z), one - two * (q.y * q.y + q.z * q.z)),
				asin(e),
				order
			};
		}
		case RotateOrder::ZXY: {
			T e = two * (q.y * q.z + q.w * q.x);
			if (e >= upperBound) // q.x = q.w, q.y = q.z
				return { pi_2, atan2(two * q.w * q.y, (q.w + q.y) * (q.w - q.y)), zero, order };
			else if (e <= lowerBound) // q.x = -q.w, q.y = -q.z
				return { -pi_2, atan2(two * q.w * q.y, (q.w + q.y) * (q.w - q.y)), zero, order };
			
			return {
				asin(e),
				atan2(two * (q.w * q.y - q.x * q.z), one - two * (q.x * q.x + q.y * q.y)),
				atan2(two * (q.w * q.z - q.x * q.y), one - two * (q.x * q.x + q.z * q.z)),
				order
			};
		}
		case RotateOrder::ZYX: {
			T e = two * (q.w * q.y - q.x * q.z);
			if (e >= upperBound) // q.z = -q.x, q.y = q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), pi_2, zero, order };
			else if (e <= lowerBound) // q.z = q.x, q.y = -q.w
				return { atan2(two * q.w * q.x, (q.w + q.x) * (q.w - q.x)), -pi_2, zero, order };
				
			return {
				atan2(two * (q.y * q.z + q.w * q.x), one - two * (q.x * q.x + q.y * q.y)),
				asin(e),
				atan2(two * (q.x * q.y + q.w * q.z), one - two * (q.y * q.y + q.z * q.z)),
				order 
			};
		}
		default:
			assert(false);
			return { }; // suppress warning C4715 in Visual Studio 
	}
}