#pragma once

#include "../math/vector2.hpp"
#include "../math/vector3.hpp"

class Texture {
public:
	Texture() = default;

	virtual ~Texture() = default;

	virtual gtm::Vector3<float> sampleColor(const gtm::Vector2<float>& texCoord) const = 0;
};