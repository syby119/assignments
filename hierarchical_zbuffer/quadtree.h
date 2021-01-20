#pragma once
#include "mesh.h"
#include "octree.h"
#include "framebuffer.h"
#include <climits>
#include <algorithm>
#include <unordered_map>
#include <glm/mat4x4.hpp>


struct QuadBoundingBox {
	int xl, xr;
	int yl, yr;
	int centerX, centerY;
};


struct QuadTreeNode {
	QuadBoundingBox* box = nullptr;
	float z = 1.0f;
	uint32_t locCode = std::numeric_limits<uint32_t>::max();
	uint8_t childExists = 0;

	QuadTreeNode() = default;
	QuadTreeNode(uint32_t LocCode) : locCode(LocCode) {}
};


class QuadTree {
public:
	/*
	 * @brief constructor
	 */
	QuadTree(int Width, int Height, Framebuffer* framebuffer);

	/*
	 * @brief destructor
     */
	~QuadTree();
	
	/*
	 * @brief search a node that can contain with 3 screen coordinates representing a triangle
	 */
	QuadTreeNode* searchNode(int* screenX, int* screenY);

	/*
	 * @brief search a node that can contain with 3 screen coordinates representing a triangle
	 */
	bool test(int* screenX, int* screenY, float z);

	/*
	 * @brief search a node that can the range of pixels
	 */
	QuadTreeNode* searchNode(int screenX, int screenY, int screenRadius);
	
	/*
	 * @brief draw a triangle with scan line
	 */
	void handleTriangle(const Triangle& tri, 
		const glm::mat4x4& model, 
		const glm::mat4x4& view, 
		const glm::mat4x4& projection, 
		const glm::vec3& ambientColor,
		const glm::vec3& lightColor,
		const glm::vec3& lightDirection);
	
	/*
	 * @brief update zbuffer
	 */
	void update(QuadTreeNode* node);
	
	/*
     * @brief clear hierarchical zbuffer data
	 */
	void clear();
	

	/*
	 * @brief get the depth of a node
	 */
	size_t getDepth(const QuadTreeNode* node) const;

	void activateHierachical(bool active);

private:
	QuadTreeNode* _root = nullptr;
	
	float* _zbuffer = nullptr;
	
	uint32_t* _indexNodeBuffer = nullptr;
	
	int _windowWidth = 0, _windowHeight = 0;

	std::unordered_map<uint32_t, QuadTreeNode> _nodes;
	
	Framebuffer* _framebuffer = nullptr;

	bool _useHierarchical = true;

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

	/*
	 * @brief clear hierarchical zbuffer data
	 */
	void _construct();


	void _splitNode(QuadTreeNode* node);

	QuadTreeNode* _getParent(QuadTreeNode* node);

	QuadTreeNode* _getNode(uint32_t locCode);

	float _processTriangle(const Triangle& tri,
		const glm::mat4x4& model, const glm::mat4x4& view, const glm::mat4x4& projection,
		int* screenX, int* screenY, float* screenZ);


	void _renderTriangle(int* screenX, int* screenY, float* screenZ, const glm::vec3& color);

	void _scanTwoLine(Side* sides, int left, int right, int dy, const glm::vec3& color);

	void _fillLine(ScanLine scanLine, const glm::vec3& color);
};