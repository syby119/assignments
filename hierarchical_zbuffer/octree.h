#pragma once

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <iostream>

#include "mesh.h"

struct OctBoundingBox {
	glm::vec3 center;
	float halfSide = 0.0f;
};

class OctreeNode {
public:
	OctBoundingBox* box = nullptr;
	std::unordered_set<Triangle*> objects;
	uint32_t locCode = std::numeric_limits<uint32_t>::max();
	uint8_t childExists = 0;
	OctreeNode() = default;
	OctreeNode(uint32_t LocCode) : locCode(LocCode) { };
};

struct OctreeZNode {
	float z = -1.0f;
	OctreeNode* node = nullptr;
};

typedef OctreeZNode* ptrOctreeZNode;

class Octree {
public:
	Octree(std::vector<Triangle>* _triangles, size_t Threshold);
	
	~Octree();
	
	void buildBoundingBox();
	
	void buildOctree();
	
	void splitNode(OctreeNode* node, int depth);

	OctreeNode* getParentNode(OctreeNode* node);
	OctreeNode* lookupNode(uint32_t locCode);
	size_t getNodeTreeDepth(const OctreeNode* node);

	OctreeNode* getRoot() { return root; }

private:
	OctreeNode* root = nullptr;
	std::unordered_map<uint32_t, OctreeNode> nodes;
	std::vector<Triangle>* objects = nullptr;
	size_t threshold = 10;
};