#pragma once

#include <cstdlib>
#include <vector>
#include <list>

#include "mesh.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>





struct ClassifiedPolyTable {
	bool inNdc(const glm::vec4& v) {
		if (-v.w <= v.x && v.x <= v.w
			&& -v.w <= v.y && v.y <= v.w
			&& -v.w <= v.z && v.z <= v.w) {
			return true;
		}
	}

	bool nearThanNearPlane(const glm::vec4& v) {
		if (v.w >= 0) {
			return true;
		} else if (v.z > -v.w){
			return true;
		}
		return false;
	}

	bool farThanFarPlane(const glm::vec4& v) {
		return false;
	}
};






struct ClassifiedEdgeTable {
	std::vector<std::list<Edge>> table;
	std::list<ActiveEdge> activeTable;

	ClassifiedEdgeTable(const std::vector<Triangle>& triangles) {

	}
};