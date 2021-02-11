#pragma once

#include <cmath>

/* function qualifier for CUDA support */
#ifdef ENABLE_CUDA
#include <cuda_runtime.h>
#define FUNC_DECL __host__ __device__
#else
#define FUNC_DECL
#endif

/* constant values */
constexpr double pi() { return 3.14159265358979323846;  }
constexpr double e() { return 2.71828182845904523536; }
constexpr double sqrt2() { return 1.41421356237309504880; }
constexpr double sqrt3() { return 1.73205080756887729353; }

template <typename T>
T radians(T deg) {
	return deg * pi() / 180.0;
}

template <typename T>
T degree(T rad) {
	return rad * 180.0 / pi();
}