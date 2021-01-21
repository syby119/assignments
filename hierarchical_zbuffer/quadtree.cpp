#include "quadtree.h"

/*
 * @brief constructor
 */
QuadTree::QuadTree(int windowWidth, int windowHeight, Framebuffer* framebuffer)
	: _windowWidth(windowWidth), _windowHeight(windowHeight), _framebuffer(framebuffer) {
	const int resolution = _windowWidth * _windowHeight;
	_zbuffer = new float[resolution];
	_indexNodeBuffer = new uint32_t[resolution];

	for (size_t i = 0; i < resolution; ++i) {
		_zbuffer[i] = 1.0f;
	}

	_root = new QuadTreeNode(1);
	_construct();
}


/*
 * @brief destructor
 */
QuadTree::~QuadTree() {
	for (auto& node : _nodes) {
		delete node.second.box;
		//delete node.second;
	}

	if (_zbuffer) {
		delete _zbuffer;
		_zbuffer = nullptr;
	}

	if (_indexNodeBuffer) {
		delete _indexNodeBuffer;
		_indexNodeBuffer = nullptr;
	}
}


/*
 * @brief clear hierarchical zbuffer data
 */
void QuadTree::clear() {
	const int resolution = _windowWidth * _windowHeight;
	for (int i = 0; i < resolution; ++i) {
		_zbuffer[i] = std::numeric_limits<float>::max();
	}

	if (_useHierarchical) {
		for (auto node : _nodes) {
			node.second.z = std::numeric_limits<float>::max();
		}
	}
}

/*
 * @brief construct the tree
 */
void QuadTree::_construct() {
	_root->box = new QuadBoundingBox{ 
		0, _windowWidth, 0, _windowHeight, (_windowWidth + 1) / 2, (_windowHeight + 1) / 2 };
	//_root->z = -10000.0f;
	_root->z = std::numeric_limits<float>::max();
	_splitNode(_root);
}


/*
 * @brief split the node recursively
 * @param node tree node to be split
 */
void QuadTree::_splitNode(QuadTreeNode* node) {
	QuadBoundingBox* box = node->box;
	// recursion stop, save the leave index into the index buffer
	if (box->xr - box->xl <= 1 && box->yr - box->yl <= 1) {
		_indexNodeBuffer[box->yl * _windowWidth + box->xl] = node->locCode;
		return;
	}

	for (int i = 0; i < 4; ++i) {
		if ((box->xr <= box->centerX && i & 1) || box->yr <= box->centerY && i & 2) {
			continue;
		}

		QuadTreeNode* nodeTemp = new QuadTreeNode((node->locCode << 2) | i);
		switch (i) {
			case 0:
				nodeTemp->box = new QuadBoundingBox{ 
					box->xl, box->centerX, box->yl, box->centerY, 
					(box->xl + box->centerX + 1) / 2, (box->yl + box->centerY + 1) / 2 
				};
				break;
			case 1:
				nodeTemp->box = new QuadBoundingBox{ 
					box->centerX, box->xr, box->yl, box->centerY, 
					(box->centerX + box->xr + 1) / 2, (box->yl + box->centerY + 1) / 2
				};
				break;
			case 2:
				nodeTemp->box = new QuadBoundingBox{ 
					box->xl, box->centerX, box->centerY, box->yr, 
					(box->xl + box->centerX + 1) / 2, (box->centerY + box->yr + 1) / 2
				};
				break;
			case 3:
				nodeTemp->box = new QuadBoundingBox{ 
					box->centerX, box->xr, box->centerY, box->yr, 
					(box->centerX + box->xr + 1) / 2, (box->centerY + box->yr + 1) / 2 
				};
				break;
		}

		nodeTemp->z = node->z;
		node->childExists |= (1 << i);
		_nodes[nodeTemp->locCode] = *nodeTemp;
	}

	// for all children
	for (int i = 0; i < 4; ++i) {
		if (node->childExists&(1 << i)) {
			const uint32_t locCodeChild = (node->locCode << 2) | i;
			auto* child = _getNode(locCodeChild);
			_splitNode(child);
		}
	}
}


/*
 * @brief search a node that can contain 3 screen coordinates representing a triangle
 */
QuadTreeNode* QuadTree::searchNode(int* screenX, int* screenY) {
	QuadTreeNode* node = _root;
	while (true) {
		uint8_t quadCode[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i) {
			quadCode[i] |= screenY[i] < node->box->centerY ? 0 : 1;
			quadCode[i] <<= 1;
			quadCode[i] |= screenX[i] < node->box->centerX ? 0 : 1;
		}

		if (quadCode[0] == quadCode[1] &&
			quadCode[1] == quadCode[2] &&
			node->childExists & (1 << quadCode[0])) {
			uint32_t locCodeChild = (node->locCode << 2) | quadCode[0];
			node = &_nodes[locCodeChild];
		} else {
			break;
		}
	}

	return node;
}


/*
 * @brief search a node that can the range of pixels
 */
QuadTreeNode* QuadTree::searchNode(int screenX, int screenY, int screenRadius) {
	QuadTreeNode* node = _root;
	
	while (true) {
		/*if ((screenY - node->box->centerY) * (screenY - node->box->centerY) +
			(screenX - node->box->centerX) * (screenX - node->box->centerX) < screenRadius * screenRadius) {
			break;
		}*/ 
		if ((screenY - node->box->centerY) * (screenY - node->box->centerY) < screenRadius * screenRadius || 
			(screenX - node->box->centerX) * (screenX - node->box->centerX) < screenRadius * screenRadius) {
			break;
		}
		else {
			uint8_t quadCode = 0;
			quadCode |= screenY < node->box->centerY ? 0 : 1;
			quadCode <<= 1;
			quadCode |= screenX < node->box->centerX ? 0 : 1;
			
			if (node->childExists & (1 << quadCode)) {
				uint32_t locCodeChild = (node->locCode << 2) | quadCode;
				node = &_nodes[locCodeChild];
			} else {
				break;
			}
		}
	}

	return node;
}

/*
 * @brief test a node that can the range of pixels
 */
bool QuadTree::test(int* screenX, int* screenY, float z) {
	QuadTreeNode* node = _root;
	while (true) {
		if (node->z < z)
			return false;
		uint8_t quadCode[3] = { 0, 0, 0 };
		for (int i = 0; i < 3; ++i) {
			quadCode[i] |= screenY[i] < node->box->centerY ? 0 : 1;
			quadCode[i] <<= 1;
			quadCode[i] |= screenX[i] < node->box->centerX ? 0 : 1;
		}

		if (quadCode[0] == quadCode[1] &&
			quadCode[1] == quadCode[2] &&
			node->childExists & (1 << quadCode[0])) {
			uint32_t locCodeChild = (node->locCode << 2) | quadCode[0];
			node = &_nodes[locCodeChild];
		}
		else {
			return true;
		}
	}
}


void QuadTree::activateHierachical(bool active) {
	_useHierarchical = active;
}



/*
 * @brief draw a triangle with scan line
 */
bool QuadTree::handleTriangle(
	const Triangle& tri, 
	const glm::mat4x4& model, 
	const glm::mat4x4& view, 
	const glm::mat4x4& projection, 
	const glm::vec3& objectColor,
	const glm::vec3& lightColor,
	const glm::vec3& lightDirection) {
	int screenX[3], screenY[3];
	float screenZ[3];
	
	float minZ = _processTriangle(tri, model, view, projection, screenX, screenY, screenZ);
	
	if (!_useHierarchical) {
		const glm::mat3x3 normalMat = glm::mat3x3(glm::transpose(inverse(model)));
		glm::vec3 ambient = 0.1f * lightColor;
		glm::vec3 norm = glm::normalize(normalMat * tri.v[0].normal);
		glm::vec3 diffuse = std::max(glm::dot(lightDirection, norm), 0.0f) * lightColor;
		glm::vec3 color = (ambient + diffuse) * objectColor;
		
		_renderTriangle(screenX, screenY, screenZ, color);
		return false;
	} else if (test(screenX, screenY, minZ)) {
		const glm::mat3x3 normalMat = glm::mat3x3(glm::transpose(inverse(model)));
		glm::vec3 ambient = 0.1f * lightColor;
		glm::vec3 norm = glm::normalize(normalMat * tri.v[0].normal);
		glm::vec3 diffuse = std::max(glm::dot(lightDirection, norm), 0.0f) * lightColor;
		glm::vec3 color = (ambient + diffuse) * objectColor;

		_renderTriangle(screenX, screenY, screenZ, color);
		return false;
	} else {
		return true;
	}
}


void QuadTree::update(QuadTreeNode* node) {
	if (node->locCode > 1) {
		QuadTreeNode* nodeParent = _getParent(node);
		float maxZ = -1.0f;

		for (int i = 0; i < 4; ++i) {
			if (nodeParent->childExists & (1 << i)) {
				uint32_t locCodeChild = nodeParent->locCode | i;
				QuadTreeNode* nodeChild = _getNode(locCodeChild);
				maxZ = std::max(maxZ, nodeChild->z);
			}
		}

		if (maxZ < nodeParent->z) {
			nodeParent->z = maxZ;
			update(nodeParent);
		}
	}
}


/*
 * @brief get the depth of a node
 */
size_t QuadTree::getDepth(const QuadTreeNode* node) const {
	int depth = 0;
	for (uint32_t lc = node->locCode; lc != 1; lc >>= 2, ++depth)
		;

	return depth;
}


float QuadTree::_processTriangle(
	const Triangle& tri,
	const glm::mat4x4& model,
	const glm::mat4x4& view,
	const glm::mat4x4& projection,
	int* screenX, int* screenY, float* screenZ) {
	float minZ = FLT_MAX;
	for (int i = 0; i < 3; ++i) {
		glm::vec4 v = projection * view * model * glm::vec4(tri.v[i].position, 1.0f);

		screenX[i] = int((v.x / v.w + 1.0f) * _windowWidth / 2);
		screenY[i] = int((v.y / v.w + 1.0f) * _windowHeight / 2);
		screenZ[i] = v.z / v.w;

		minZ = std::min(minZ, screenZ[i]);
	}

	return minZ;
}


void QuadTree::_renderTriangle(int* screenX, int* screenY, float* screenZ, const glm::vec3& color) {
	// sort the edge of the triangle
	Side sides[3];
	for (int i = 0; i < 3; ++i) {
		int index1 = (i + 1) % 3;
		int index2 = (i + 2) % 3;
		bool flag;
		if (screenY[index1] == screenY[index2]) {
			flag = screenX[index1] < screenX[index2] ? true : false;
		} else {
			flag = screenY[index1] < screenY[index2] ? true : false;
		}

		sides[i].yMin = flag ? screenY[index1] : screenY[index2];
		sides[i].x = flag ? screenX[index1] : screenX[index2];
		sides[i].z = flag ? screenZ[index1] : screenZ[index2];
		sides[i].dy = (screenY[index2] - screenY[index1])*(flag ? 1 : -1);
		sides[i].dx = (screenX[index2] - screenX[index1])*(flag ? 1 : -1) * 1.0f / sides[i].dy;
		sides[i].dz = (screenZ[index2] - screenZ[index1])*(flag ? 1 : -1) * 1.0f / sides[i].dy;
	}

	std::sort(sides, sides + 3, [](const Side& a, const Side& b) {
		return a.yMin < b.yMin;
	});

	// if the triangle has a horizontal edge
	if (sides[0].yMin == sides[1].yMin && sides[1].yMin == sides[2].yMin) {
		int index = 0;
		for (; index < 3; ++index) {
			if (sides[index].dy == 0)
				break;
		}
		int index1 = (index + 1) % 3;
		int index2 = (index + 2) % 3;
		bool flag = sides[index1].x < sides[index2].x ? true : false;
		int left = flag ? index1 : index2;
		int right = flag ? index2 : index1;
		int dy = sides[left].dy;
		_scanTwoLine(sides, left, right, dy, color);
	} else {
		bool flag = sides[0].dx < sides[1].dx ? true : false;
		int left = flag ? 0 : 1;
		int right = flag ? 1 : 0;
		int dy = std::min(sides[left].dy, sides[right].dy);
		_scanTwoLine(sides, left, right, dy, color);
		int index;
		if (sides[left].dy < sides[right].dy)
			left = 2, index = right;
		else if (sides[left].dy > sides[right].dy)
			right = 2, index = left;
		else
			return;

		sides[index].yMin += dy;
		sides[index].x += dy * sides[index].dx;
		sides[index].z += dy * sides[index].dz;
		sides[index].dy -= dy;
		dy = sides[index].dy;
		_scanTwoLine(sides, left, right, dy, color);
	}
}


void QuadTree::_scanTwoLine(Side* sides, int left, int right, int dy, const glm::vec3& color) {
	ScanLine scanLine;
	float xl = sides[left].x;
	float xr = sides[right].x;
	scanLine.zl = sides[left].z;
	scanLine.y = sides[left].yMin;
	
	for (int i = 0; i < dy; ++i) {
		scanLine.xl = (int)xl;
		scanLine.xr = (int)xr;
		scanLine.dz = (sides[right].z + i * sides[right].dz - scanLine.zl) / (scanLine.xr - scanLine.xl);

		if (scanLine.y >= 0 && scanLine.y < _windowHeight) {
			_fillLine(scanLine, color);
		}

		xl += sides[left].dx;
		xr += sides[right].dx;
		scanLine.zl += sides[left].dz;
		++scanLine.y;
	}
}


void QuadTree::_fillLine(ScanLine scanline, const glm::vec3& color) {
	int y = scanline.y;
	float z = scanline.zl;
	int index = _windowWidth * y + scanline.xl;

	for (int x = scanline.xl; x <= scanline.xr; ++x) {
		if (x >= 0 && x < _windowWidth && z < _zbuffer[index] && z >= -1.0f) {
			_zbuffer[index] = z;
			_framebuffer->setPixel(x, y, color);

			QuadTreeNode* node = &_nodes[_indexNodeBuffer[index]];
			node->z = z;
			if (_useHierarchical == true) {
				update(node);
			}
		}
		z += scanline.dz;
		++index;
	}
}


QuadTreeNode* QuadTree::_getParent(QuadTreeNode* node) {
	const uint32_t locCodeParent = node->locCode >> 2;
	return _getNode(locCodeParent);
}


QuadTreeNode* QuadTree::_getNode(uint32_t locCode) {
	return &_nodes[locCode];
}


void testAndSet(int x, int y, float z) {
	
}

