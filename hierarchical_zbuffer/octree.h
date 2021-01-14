#pragma once
#include "mesh.h"
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iostream>

struct OctBoundingBox {
	glm::vec3 center;
	float halfSide;
};

class OctreeNode {
public:
	OctBoundingBox* box;
	std::unordered_set<Triangle*> objects;
	uint32_t locCode;
	uint8_t childExists;
	OctreeNode() {}
	OctreeNode(uint32_t LocCode) {
		locCode = LocCode;
		childExists = 0;
	}
};

struct OctreeZNode {
	float z;
	OctreeNode* node;
};

typedef OctreeZNode* ptrOctreeZNode;

class Octree {
public:
	Octree(std::vector<Triangle>* _triangles, size_t Threshold);
	void buildBoundingBox();
	void buildOctree();
	void splitNode(OctreeNode* node);

	OctreeNode* getParentNode(OctreeNode* node);
	OctreeNode* lookupNode(uint32_t locCode);
	size_t getNodeTreeDepth(const OctreeNode* node);

	OctreeNode* getRoot() { return root; }

private:
	OctreeNode* root;
	std::unordered_map<uint32_t, OctreeNode> nodes;
	std::vector<Triangle>* objects;
	size_t threshold;
};