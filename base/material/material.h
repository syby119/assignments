#pragma once

#include "../core/ray.h"
#include "../core/ray_hittable.h"
#include "../math/vector3.hpp"

class Material {
public:
	Material() = default;

	virtual ~Material() = default;

	virtual bool scatter(
		const Ray& rayIn, 
		const RayHitInfo& info, 
		gtm::Vector3<float>& attenuation, 
		Ray& rayOut) const = 0;
};