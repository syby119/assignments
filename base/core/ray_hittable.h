#pragma once

#include <memory>

#include "../math/vector2.hpp"
#include "../math/vector3.hpp"
#include "./ray.h"


class Material;

struct RayHitInfo {
	float t;
	gtm::Vector3<float> point;
	gtm::Vector3<float> normal;
	gtm::Vector2<float> texCoord;
	std::shared_ptr<Material> materials;
};

class RayHittable {
public:
	virtual bool hit(const Ray& ray, float tmin, float tmax, RayHitInfo& info) = 0;
};