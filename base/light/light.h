#pragma once

#include "object3d.h"

class Light : Object3D {
public:
	Light() = default;
	~Light() = default;
private:
	float _intensity = 1.0f;
	float _exposure = 0.0f;
	float _decay = 1.0f;

	glm::vec3 _color;
};