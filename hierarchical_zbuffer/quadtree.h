#pragma once
#include "mesh.h"
#include <climits>
#include <algorithm>
#include <unordered_map>

#include <glm/mat4x4.hpp>

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
	QuadTreeNode() {}
	QuadTreeNode(uint32_t LocCode) {
		locCode = LocCode;
		childExists = 0;
	}
};

class QuadTree {
public:
	QuadTree(int Width, int Height);
	void buildQuadTree();
	void splitNode(QuadTreeNode* node);
	float calTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection, int* screenX, int* screenY);
	QuadTreeNode* searchNode(int* screenX, int* screenY);

	QuadTreeNode* getParentNode(QuadTreeNode* node);
	QuadTreeNode* lookupNode(uint32_t locCode);
	size_t getNodeTreeDepth(const QuadTreeNode* node);

private:
	QuadTreeNode* root;
	float* frameBuffer;
	float* zBuffer;
	uint32_t* indexNodeBuffer;
	int width, height;
	std::unordered_map<uint32_t, QuadTreeNode> nodes;
};