#pragma once

#include "../math/vector3.hpp"

struct Ray {
public:
	Ray(const gtm::Vector3<float>& origin, const gtm::Vector3<float>& direction)
		: origin(origin), direction(direction) { }
	~Ray() { }

	gtm::Vector3<float> point(float t) { return origin + t * direction; }

public:
	gtm::Vector3<float> origin;
	gtm::Vector3<float> direction;
};