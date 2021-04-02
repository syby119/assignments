#pragma once

#include <memory>

#include "./material.h"
#include "../texture/texture.h"
#include "../texture/solid_texture.h"
#include "../texture/image_texture.h"

class Lambertian : public Material {
public:
	Lambertian(gtm::Vector3<float>& color)
		: _albedo(std::make_shared<SolidTexture>(color)) {}

	Lambertian(std::shared_ptr<Texture> albedo): _albedo(albedo) {}

	bool scatter(
		const Ray& rayIn,
		const RayHitInfo& info,
		gtm::Vector3<float>& attenuation,
		Ray& rayOut) const override {
		attenuation = ;
	}
	
	~Lambertian() {}

private:
	std::shared_ptr<Texture> _albedo;
};