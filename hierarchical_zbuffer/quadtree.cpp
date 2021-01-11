#include "quadtree.h"

QuadTree::QuadTree(int Width, int Height) {
	width = Width, height = Height;
	frameBuffer = new float[width*height];
	zBuffer = new float[width*height];
	indexNodeBuffer = new uint32_t[width*height];
	memset(zBuffer, FLT_MIN, width*height * sizeof(float));
	root = new QuadTreeNode(1);

	buildQuadTree();
}

void QuadTree::buildQuadTree() {
	root->box = new QuadBoundingBox{ 0, width, 0, height, (width + 1) / 2, (height + 1) / 2 };
	root->z = FLT_MIN;
	splitNode(root);
}

void QuadTree::splitNode(QuadTreeNode* node) {
	QuadBoundingBox* box = node->box;
	// 存储叶子节点索引到indexNodeBuffer
	if (box->xr - box->xl <= 1 && box->yr - box->yl <= 1) {
		indexNodeBuffer[box->yl * width + box->xl] = node->locCode;
		return;
	}

	for (int i = 0; i < 4; ++i) {
		if ((box->xr <= box->centerX && i & 1) ||
			box->yr <= box->centerY && i & 2)
			continue;
		QuadTreeNode* nodeTemp = new QuadTreeNode((node->locCode << 2) | i);
		switch (i) {
		case 0:
			nodeTemp->box = new QuadBoundingBox{ box->xl, box->centerX, box->yl, box->centerY, (box->xl + box->centerX + 1) / 2, (box->yl + box->centerY + 1) / 2 };
			break;
		case 1:
			nodeTemp->box = new QuadBoundingBox{ box->centerX, box->xr, box->yl, box->centerY, (box->centerX + box->xr + 1) / 2, (box->yl + box->centerY + 1) / 2 };
			break;
		case 2:
			nodeTemp->box = new QuadBoundingBox{ box->xl, box->centerX, box->centerY, box->yr, (box->xl + box->centerX + 1) / 2, (box->centerY + box->yr + 1) / 2 };
			break;
		case 3:
			nodeTemp->box = new QuadBoundingBox{ box->centerX, box->xr, box->centerY, box->yr, (box->centerX + box->xr + 1) / 2, (box->centerY + box->yr + 1) / 2 };
			break;
		}
		nodeTemp->z = node->z;
		node->childExists |= (1 << i);
		nodes[nodeTemp->locCode] = *nodeTemp;
	}
	// 对子节点迭代
	for (int i = 0; i < 3; ++i) {
		if (node->childExists&(1 << i)) {
			const uint32_t locCodeChild = (node->locCode << 2) | i;
			// splitNode(&nodes[locCodeChild]);
			auto* child = lookupNode(locCodeChild);
			splitNode(child);
		}
	}
}



float QuadTree::calTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection, int* screenX, int* screenY) {
	float maxZ = FLT_MIN;
	for (int i = 0; i < 3; ++i) {
		glm::vec4 Vv = view * glm::vec4(tri.v[i].position, 1.0);
		maxZ = std::max(maxZ, Vv.z);
		glm::vec4 PVv = projection * Vv;
		screenX[i] = int((PVv.x / PVv.w + 1.0f) * width / 2);
		screenY[i] = int((PVv.y / PVv.w + 1.0f) * height / 2);
	}
	return maxZ;
}

QuadTreeNode* QuadTree::searchNode(int* screenX, int* screenY) {
	bool flag = true;
	QuadTreeNode* node = root;
	while (flag) {
		uint8_t quadCode[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i) {
			quadCode[i] |= screenX[i] < node->box->centerX ? 0 : 1;
			quadCode[i] <<= 1;
			quadCode[i] |= screenY[i] < node->box->centerY ? 0 : 1;
		}
		if (quadCode[0] == quadCode[1] &&
			quadCode[1] == quadCode[2] &&
			node->childExists&(1<<quadCode[0])) {
			uint32_t locCodeChild = node->locCode << 2 | quadCode[0];
			node = &nodes[locCodeChild];
		}
		else {
			flag = false;
		}
	}
	return node;
}

QuadTreeNode* QuadTree::getParentNode(QuadTreeNode* node) {
	const uint32_t locCodeParent = node->locCode >> 2;
	return lookupNode(locCodeParent);
}

QuadTreeNode* QuadTree::lookupNode(uint32_t locCode) {
	return &nodes[locCode];
}

size_t QuadTree::getNodeTreeDepth(const QuadTreeNode* node) {
	int depth = 0;
	for (uint32_t lc = node->locCode; lc != 1; lc >>= 2, ++depth);
	return depth;
}