#pragma once

#include <list>
#include <vector>

#include <glm/mat4x4.hpp>

#include "mesh.h"
#include "model.h"
#include "camera.h"
#include "clipper.h"
#include "zbuffer.h"
#include "quadtree.h"
#include "octree.h"
#include "framebuffer.h"
#include "scanline_renderer.h"

struct Polygon {
	float a, b, c, d;
	// id of the triangle
	int id;
	// number of scan line contained
	int dy;
	// render color of the polygon
	glm::vec3 color;
};


struct Edge {
	/* top point x of the edge */
	int x;
	/* 1 / k */
	float dx;
	/* scan line contained */
	int dy;
	/* id of the triangle */
	float id;
};


struct ActiveEdgePair {
	/* x coordinate of left/right point the scan line intersect */
	float xl, xr;
	/* x increment when move 1 pixel in y direction */
	float dxl, dxr;
	/* scanline contained */
	int dyl, dyr;
	float zl;
	/* z increasement per pixel in x/y direction, dzx = -a/c, dzy = b/c */
	float dzx, dzy;
	/* index of the triangle */
	int id;
};

class ScanlineRenderer {
public:
	enum class RenderMode {
		ZBuffer,
		HierarchicalZBuffer,
		OctreeHierarchicalZBuffer
	};

	ScanlineRenderer(Framebuffer& framebuffer,
		int windowWidth, int windowHeight,
		std::vector<Triangle>& triangles,
		const glm::vec4& clearColor);

	void render(
		Framebuffer& framebuffer,
		const Camera& camera,
		const std::vector<Model>& models,
		const glm::vec3& objectColor,
		const glm::vec3& lightColor,
		const glm::vec3& lightDirection);

	void setRenderMode(enum RenderMode renderMode);

private:
	/* render mode */
	RenderMode _renderMode = RenderMode::ZBuffer;

	Framebuffer& _framebuffer;

	/* image resolution */
	int _windowWidth = 0;
	int _windowHeight = 0;

	/* clear color */
	glm::vec4 _clearColor;

	/* clipper */
	Clipper _clipper;

	/* classified polygon table */
	std::vector<std::list<Polygon>> _classifiedPolygonTable;

	/* classified edge table */
	std::vector<std::list<Edge>> _classifiedEdgeTable;

	/* active polygon table */
	std::list<Polygon> _activePolygonTable;

	/* active edge table */
	std::list<ActiveEdgePair> _activeEdgeTable;

	/* triangles */
	std::vector<Triangle>& _triangles;

	/* zbuffer */
	Zbuffer* _zbuffer = nullptr;

	/* hierachical zbuffer */
	QuadTree* _quadTree = nullptr;

	/* octree */
	Octree* _octree = nullptr;

	/*
	 * @brief assemble classified polygon table and classified edge table
	 */
	void _assembleRenderData(const Camera& camera,
				   const std::vector<Model>& models,
				   const glm::vec3& objectColor,
				   const glm::vec3& lightColor,
				   const glm::vec3& lightDirection);

	/*
	 * @brief clear scan line data structure rendered
	 */
	void _clearRenderData();

	void _scan(Framebuffer& framebuffer);

	void _renderWithScanLineZBuffer();

	void _renderWithHierarchicalZBuffer(const Camera& camera);

	void _renderWithOctreeHierarchicalZBuffer(const Camera& camera);
};

