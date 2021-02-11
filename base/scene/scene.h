#pragma once

#include <vector>

#include "../camera/camera.h"
#include "../light/light.h"
#include "model.h"


class Scene {
public:
	void add(const Model& model);
	void add(const Light& light);
	void add(const Camera& camera);
private:
	std::vector<Model> _models;
	std::vector<Light> _lights;
	std::vector<Camera> _cameras;
};
