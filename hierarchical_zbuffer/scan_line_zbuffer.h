#pragma once

#include <cstdlib>
#include <vector>
#include <list>

#include "mesh.h"
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>


struct Polygon {
	/* coefficient of the plane equation */
	float a, b, c, d;
	/* index of the triangle */
	float id;
	/* num of scan line contained */
	int dy;
	/* color */
	glm::vec3 color;
};


struct ClassifiedPolyTable {
	std::vector<std::list<Polygon>> table;
	std::list<Polygon> activeTable;

	ClassifiedPolyTable(const std::vector<Triangle>& triangles, 
						const glm::mat4x4& model,
						const glm::mat4x4& view,
						const glm::mat4x4& projection,
						uint32_t xResolution, uint32_t yResolution)
		: table(yResolution) {
		const glm::mat4 modelView = view * model;


		for (size_t i = 0; i < triangles.size(); ++i) {
			// transform

			glm::vec4 v[3];
			for (int j = 0; j < 3; ++j) {
				v[i] = projection * model * glm::vec4(triangles[i].v[0].position, 1.0f);
			}

			triangles[i].v[]

			// clip
			//if (inNdc(v[0]) && inNdc(v[1]) && inNdc(v[2])) {
			//	// whole triangle in ndc
			//	color = ;
			//	continue;
			//} 

			//if (nearThanNearPlane(v[0]) && nearThanNearPlane(v[1]) && nearThanNearPlane(v[2])) {
			//	// whole triangle is at the invisible near side of the camera
			//	continue;
			//}

			//if (farThanFarPlane(v[0]) && farThanFarPlane(v[1]) && farThanFarPlane(v[2])) {
			//	// the whole triangle is at the invisible far side of the camera
			//	continue;
			//}

			

		}
	}

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


struct Edge {
	/* top point x of the edge */
	int x;
	/* 1 / k */
	float dx;
	/* scan line contained */
	int dy;
	/* index of the triangle */
	float index;
};


struct ActiveEdge {
	/* x coordinate of left/right point the scan line intersect */
	int xl, xr;
	/* x coordinate of left/right point the scan line intersect */
	float dxl, dxr;
	int dyl, dyr;
	float zl;
	/* z increasement per pixel in x/y direction, dzx = -a/c, dzy = b/c */
	float dzx, dzy;
	/* index of the triangle */
	int id;
};


struct ClassifiedEdgeTable {
	std::vector<std::list<Edge>> table;
	std::list<ActiveEdge> activeTable;

	ClassifiedEdgeTable(const std::vector<Triangle>& triangles) {

	}
};