#pragma once

#include "./texture.h"
#include "../math/vector3.hpp"

class SolidTexture : public Texture {
public:
	SolidTexture(const gtm::Vector3<float>& color) : _color(color) { }

	~SolidTexture() = default;

	gtm::Vector3<float> sampleColor(const gtm::Vector2<float>& texCoord) const override {
		return _color;
	}

private:
	gtm::Vector3<float> _color;
};