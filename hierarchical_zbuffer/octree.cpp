#include "octree.h"

Octree::Octree(std::vector<Triangle>* _triangles, size_t Threshold) {
	objects = _triangles;
	threshold = Threshold;
	root = new OctreeNode(1);
	buildBoundingBox();
	buildOctree();
}

void Octree::buildBoundingBox() {
	glm::vec3 vertexMax{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	glm::vec3 vertexMin{ FLT_MAX, FLT_MAX, FLT_MAX };
	for (int i = 0; i < objects->size(); ++i) {
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k)
			{
				vertexMax[k] = std::max((*objects)[i].v[j].position[k], vertexMax[k]);
				vertexMin[k] = std::min((*objects)[i].v[j].position[k], vertexMin[k]);
			}
		}
		root->objects.insert(&(*objects)[i]);
	}
	root->box = new OctBoundingBox();
	root->box->center = (vertexMax + vertexMin) * 0.5f;
	glm::vec3 deltaHalf = root->box->center - vertexMin;
	root->box->halfSide = deltaHalf[0];
	root->box->halfSide = std::max(deltaHalf[1], root->box->halfSide);
	root->box->halfSide = std::max(deltaHalf[2], root->box->halfSide);
}

void Octree::buildOctree() {
	nodes[root->locCode] = *root;
	splitNode(root);
}

void Octree::splitNode(OctreeNode* node) {
	if (node->objects.size() < threshold)
		return;
	glm::vec3 center = node->box->center;
	float halfSide = node->box->halfSide;
	for (auto iter = node->objects.begin(); iter != node->objects.end(); ) {
		uint32_t locCodeTemp[3] = {0, 0, 0};
		for (int i = 0; i < 3; ++i) {
			locCodeTemp[i] |= (*iter)->v[i].position[0] > center[0] ? 1 : 0;
			locCodeTemp[i] <<= 1;
			locCodeTemp[i] |= (*iter)->v[i].position[1] > center[1] ? 1 : 0;
			locCodeTemp[i] <<= 1;
			locCodeTemp[i] |= (*iter)->v[i].position[2] > center[2] ? 1 : 0;
		}
		if (locCodeTemp[0] == locCodeTemp[1] &&
			locCodeTemp[1] == locCodeTemp[2]) {
			// 构建新的子树node
			const uint32_t locCodeChild = (node->locCode << 3) | locCodeTemp[0];
			std::cout << "locCodeChild: " << locCodeChild << std::endl;
			std::cout << "Depth child: " << getNodeTreeDepth(&nodes[locCodeChild]) << std::endl;
			// 构造一个新节点
			if (!(node->childExists&(1 << locCodeTemp[0]))) {
				node->childExists |= 1 << (locCodeTemp[0]);
				auto childNode = new OctreeNode(locCodeChild);
				glm::vec3 childCenter;
				float halfHalfSide = halfSide * 0.5f;
				childCenter.x = center.x + (locCodeTemp[0] & 4) ? halfHalfSide : -halfHalfSide;
				childCenter.y = center.y + (locCodeTemp[0] & 2) ? halfHalfSide : -halfHalfSide;
				childCenter.z = center.z + (locCodeTemp[0] & 1) ? halfHalfSide : -halfHalfSide;
				childNode->box = new OctBoundingBox{childCenter, halfHalfSide };
				nodes[childNode->locCode] = *childNode;
			}
			// 赋予三角形
			auto *child = lookupNode(locCodeChild);
			child->objects.insert(*iter);
			node->objects.erase(iter++);
			// move triangle
		}
		else {
			++iter;
		}
	}

	// 对子节点迭代
	for (int i = 0; i < 8; ++i) {
		if (node->childExists&(1 << i)) {
			const uint32_t locCodeChild = (node->locCode << 3) | i;
			auto* child = lookupNode(locCodeChild);
			splitNode(child);
		}
	}
}

OctreeNode* Octree::getParentNode(OctreeNode* node) {
	const uint32_t locCodeParent = node->locCode >> 3;
	return lookupNode(locCodeParent);
}

OctreeNode* Octree::lookupNode(const uint32_t locCode) {
	return &nodes[locCode];
}

size_t Octree::getNodeTreeDepth(const OctreeNode* node) {
	int depth = 0;
	for (uint32_t lc = node->locCode; lc != 1; lc >>= 3, ++depth);
	return depth;
}