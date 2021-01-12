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
};


struct ClassifiedPolyTable {
	std::vector<std::list<Polygon>> table;
	std::list<Polygon> activeTable;

	ClassifiedPolyTable(const std::vector<Triangle>& triangles, 
						const glm::mat4x4& mvp,
						uint32_t xResolution, uint32_t yResolution)
		: table(yResolution) {
		for (size_t i = 0; i < triangles.size(); ++i) {
			// transform
			glm::vec4 v[3];
			for (int j = 0; j < 3; ++j) {
				v[i] = mvp * glm::vec4(triangles[i].v[0].position, 1.0f);
			}
			// clip
			if (v[0].w > 0 && v[0].x < v[0].w && v[0].y < v[0].w && v[0].w && v[0].z < v[0].w) {

			}
		}
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