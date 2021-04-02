#pragma once

#include "./ray.h"
#include "../math/vector3.hpp"

class Material {
public:
	Material();
	~Material();

	virtual bool scatter(const Ray& ray, const, const Hit);
	virtual bool emit()

protected:

};