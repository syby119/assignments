#pragma once

#include "./material.h"
#include "../core/ray.h"
#include "../math/vector3.hpp"

class Metal : public Material {
public:
	Metal(const gtm::Vector3<float>& albedo, const float fuzz)
		:_albedo(albedo), _fuzz(fuzz) { }

	~Metal() = default;

	bool scatter(
		const Ray& rayIn,
		const RayHitInfo& info,
		gtm::Vector3<float>& attenuation,
		Ray& rayOut) const override {

	}

private:
	gtm::Vector3<float> _albedo;
	float _fuzz;
};