#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fov, float aspect, float znear = 0.1f, float zfar = 10000.0f)
		: Camera(fov, aspect, znear, zfar) { }

	glm::mat4x4 getProjectionMatrix() const override {
		return glm::perspective(_fov, _aspect, _znear, _zfar);
	}
};