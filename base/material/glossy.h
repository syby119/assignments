#pragma once

#include "./material.h"
#include "../math/vector3.hpp"
#include "../texture/texture.h"
#include "../texture/solid_texture.h"
#include "../texture/image_texture.h"

class Glossy : public Material {
public:
	Glossy();
	~Glossy();

	bool scatter(
		const Ray& rayIn,
		const RayHitInfo& info,
		gtm::Vector3<float>& attenuation,
		Ray& rayOut) const override {
		rayOut.origin = rayIn.origin;
		rayOut.direction = 
		attenuation = _kd * _albedo->sampleColor(info.texCoord) + _ks * info.normal;

	}
private:
	std::shared_ptr<Texture> _albedo;
	gtm::Vector3<float> _kd;
	gtm::Vector3<float> _ks;
	float _ns;
};