#pragma once
#include "mesh.h"
#include "octree.h"
#include "framebuffer.h"
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <glm/mat4x4.hpp>
//#include <iostream>


struct Side {
	int yMin;
	int x;
	float z;
	int dy;
	float dx;
	float dz;
};

struct ScanLine {
	int xl, xr;
	float zl;
	int y;
	float dz;
};

struct QuadBoundingBox {
	int xl, xr;
	int yl, yr;
	int centerX, centerY;
};

class QuadTreeNode {
public:
	QuadBoundingBox* box;
	float z;
	uint32_t locCode;
	uint8_t childExists;
	QuadTreeNode() = default;
	QuadTreeNode(uint32_t LocCode) {
		locCode = LocCode;
		childExists = 0;
	}
};

class QuadTree {
public:
	QuadTree(int Width, int Height, Framebuffer* framebuffer);
	void buildQuadTree();
	void splitNode(QuadTreeNode* node);
	
	QuadTreeNode* searchNode(int* screenX, int* screenY);
	QuadTreeNode* searchNode(int screenX, int screenY, int screenRadius);
	void handleTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection);
	float calTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection, int* screenX, int* screenY, float* screenZ);
	void renderTriangle(int* screenX, int* screenY, float* screenZ, glm::vec3 color);
	void scanTwoLine(Side* sides, int left, int right, int dy, glm::vec3 color);
	void render(ScanLine scanLine, glm::vec3 color);
	void updateQuadTree(QuadTreeNode* node);
	void clearZBuffer();

	QuadTreeNode* getParentNode(QuadTreeNode* node);
	QuadTreeNode* lookupNode(uint32_t locCode);
	size_t getNodeTreeDepth(const QuadTreeNode* node);

	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	glm::vec3 ambientColor;
	
public:
	QuadTreeNode* root;
	float* zBuffer;
	uint32_t* indexNodeBuffer;
	int width, height;
	std::unordered_map<uint32_t, QuadTreeNode> nodes;
	Framebuffer* frameBuffer;
};