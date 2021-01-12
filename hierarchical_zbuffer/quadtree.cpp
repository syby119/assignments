#include "quadtree.h"

bool cmp(Side a, Side b) {
	return a.yMin < b.yMin;
};

QuadTree::QuadTree(int Width, int Height) {
	width = Width, height = Height;
	lightDirection = glm::normalize(glm::vec3(0.8f, -3.0f, 1.5f));
	lightColor = glm::vec3(0.6f, 0.7f, 0.8f);
	frameBuffer = new glm::vec3[width*height];
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

float QuadTree::calTriangle(Triangle& tri, glm::mat4x4& view, glm::mat4x4& projection, int* screenX, int* screenY, float* screenZ) {
	float maxZ = FLT_MIN;
	for (int i = 0; i < 3; ++i) {
		glm::vec4 Vv = view * glm::vec4(tri.v[i].position, 1.0);
		screenZ[i] = Vv.z;
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

void QuadTree::renderTriangle(int* screenX, int* screenY, float* screenZ, glm::vec3 color) {
	// 提取三条边并排序
	Side sides[3];
	for (int i = 0; i < 3; ++i) {
		int index1 = (i + 1) % 3;
		int index2 = (i + 2) % 3;
		bool flag;
		if (screenY[index1] == screenY[index2]) 
			flag = screenX[index1] < screenX[index2] ? true : false;
		else
			flag = screenY[index1] < screenY[index2] ? true : false;
		sides[i].yMin = flag ? screenY[index1] : screenY[index2];
		sides[i].x = flag ? screenX[index1] : screenX[index2];
		sides[i].z = flag ? screenZ[index1] : screenZ[index2];
		sides[i].dy = (screenY[index2] - screenY[index1])*(flag ? 1 : -1);
		sides[i].dx = (screenX[index2] - screenX[index1])*(flag ? 1 : -1)*1.0f / sides[i].dy;
		sides[i].dz = (screenZ[index2] - screenZ[index1])*(flag ? 1 : -1)*1.0f / sides[i].dy;
	}
	std::sort(sides, sides + 3, cmp);

	// 如果三角形有一条上面的水平边
	if (sides[0].yMin == sides[1].yMin && sides[1].yMin == sides[2].yMin) {
		int index = 0;
		for (; index < 3; ++index) {
			if (sides[index].dy == 1)
				break;
		}
		ScanLine scanLine;
		int index1 = (index + 1) % 3;
		int index2 = (index + 2) % 3;
		bool flag = sides[index1].x < sides[index2].x ? true : false;
		int left = flag ? index1 : index2;
		int right = flag ? index2 : index1;
		int dy = sides[left].dy;
		scanTwoLine(sides, left, right, dy, color);
	}
	else {
		bool flag = sides[0].dx < sides[1].dx ? true : false;
		int left = flag ? 0 : 1;
		int right = flag ? 1 : 0;
		int dy = std::min(sides[left].dy, sides[right].dy);
		scanTwoLine(sides, left, right, dy, color);
		int index;
		if (sides[left].dy < sides[right].dy)
			left = 2, index = right;
		else if(sides[left].dy > sides[right].dy)
			right = 2, index = left;
		sides[index].yMin += dy;
		sides[index].x += dy * sides[index].dx;
		sides[index].z += dy * sides[index].dz;
		sides[index].dy -= dy;
		dy = sides[index].dy;
		scanTwoLine(sides, left, right, dy, color);
	}
}

void QuadTree::scanTwoLine(Side* sides, int left, int right, int dy, glm::vec3 color) {
	ScanLine scanLine;
	for (int i = 0; i < dy; ++i) {
		scanLine.xl = sides[left].x + (int)(i*sides[left].dx);
		scanLine.xr = sides[right].x + (int)(i*sides[right].dx);
		scanLine.zl = sides[left].z + (int)(i*sides[left].dz);
		scanLine.y = sides[left].yMin + i;
		scanLine.dz = (sides[right].z + i * sides[right].dz - sides[left].z - i * sides[left].dz) / (scanLine.xr - scanLine.xl);
		render(scanLine, color);
	}
	return;
}

void QuadTree::render(ScanLine scanline, glm::vec3 color) {
	int y = scanline.y;
	for (int x = scanline.xl; x < scanline.xr; ++x) {
		float z = scanline.zl + x * scanline.dz;
		int index = width * y + z;
		if (z > zBuffer[index]) {
			zBuffer[index] = z;
			frameBuffer[index] = color;
			QuadTreeNode* node = &nodes[indexNodeBuffer[index]];
			node->z = z;
			updateQuadTree(node);
		}
	}
}

void QuadTree::updateQuadTree(QuadTreeNode* node) {
	// 更新整棵树
	if (node->locCode != 1) {
		uint32_t locCodeParent = node->locCode >> 2;
		QuadTreeNode* nodeParent = &nodes[locCodeParent];
		float minZ = FLT_MAX;
		for (int i = 0; i < 4; ++i) {
			if(nodeParent->childExists&(1 << i)) {
				uint32_t locCodeChild = locCodeParent | i;
				QuadTreeNode* nodeChild = &nodes[locCodeChild];
				minZ = std::min(minZ, nodeChild->z);
			}
		}
		if (minZ > nodeParent->z)
			updateQuadTree(nodeParent);
	}
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