#pragma once
#include "mesh.h"
#include "framebuffer.h"
#include <climits>
#include <algorithm>
#include <unordered_map>
//#include <queue>
//#include <fstream>
#include <glm/mat4x4.hpp>


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

bool cmp(Side a, Side b);

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
	float calTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection, int* screenX, int* screenY, float* screenZ);
	QuadTreeNode* searchNode(int* screenX, int* screenY);
	void renderTriangle(int* screenX, int* screenY, float* screenZ, glm::vec3 color, Framebuffer& framebuffer);
	void scanTwoLine(Side* sides, int left, int right, int dy, glm::vec3 color, Framebuffer& framebuffer);
	void render(ScanLine scanLine, glm::vec3 color, Framebuffer& framebuffer);
	void updateQuadTree(QuadTreeNode* node);

	QuadTreeNode* getParentNode(QuadTreeNode* node);
	QuadTreeNode* lookupNode(uint32_t locCode);
	size_t getNodeTreeDepth(const QuadTreeNode* node);

	glm::vec3* getFrameBuffer() { return frameBuffer; }
	//QuadTreeNode* getRoot() { return root; }
	glm::vec3 lightDirection;
	glm::vec3 lightColor;
	
private:
	QuadTreeNode* root;
	glm::vec3* frameBuffer;
	float* zBuffer;
	uint32_t* indexNodeBuffer;
	int width, height;
	std::unordered_map<uint32_t, QuadTreeNode> nodes;
};