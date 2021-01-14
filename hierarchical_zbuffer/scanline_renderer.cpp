#include <iostream>
#include "scanline_renderer.h"


ScanlineRenderer::ScanlineRenderer(
	Framebuffer& framebuffer,
	int windowWidth, int windowHeight,
	std::vector<Triangle>& triangles,
	const glm::vec4& clearColor)
	: _framebuffer(framebuffer),
	  _windowWidth(windowWidth), _windowHeight(windowHeight),
	  _clearColor(clearColor), 
		_triangles(triangles) {
	_classifiedPolygonTable.resize(windowHeight);
	_classifiedEdgeTable.resize(windowHeight);
	_zbuffer = new Zbuffer(windowWidth, windowHeight);
	_quadTree = new QuadTree(windowWidth, windowHeight, &_framebuffer);
	_octree = new Octree(&triangles, 10);
}


void ScanlineRenderer::render(
	Framebuffer &framebuffer,
	const Camera& camera,
	const std::vector<Model>& models,
	const glm::vec3& objectColor,
	const glm::vec3& lightColor,
	const glm::vec3& lightDirection) {
	if (_renderMode == RenderMode::ZBuffer) {
		std::cout << "not ready" << std::endl;
		_clearRenderData();
		framebuffer.clear(_clearColor);

		_assembleRenderData(camera, models, objectColor, lightColor, lightDirection);

		_scan(framebuffer);
	} else if (_renderMode == RenderMode::HierarchicalZBuffer) {
		_renderWithHierarchicalZBuffer(camera);
	} else {
		_renderWithOctreeHierarchicalZBuffer(camera);
	}
}


void ScanlineRenderer::setRenderMode(enum RenderMode renderMode) {
	_renderMode = renderMode;
}


/*
 * @brief clear scan line data structure rendered
 */
void ScanlineRenderer::_clearRenderData() {
	for (size_t i = 0; i < _classifiedPolygonTable.size(); ++i) {
		_classifiedPolygonTable[i].clear();
	}

	for (size_t i = 0; i < _classifiedPolygonTable.size(); ++i) {
		_classifiedEdgeTable[i].clear();
	}

	_activePolygonTable.clear();
	_activeEdgeTable.clear();

	switch (_renderMode) {
	case RenderMode::ZBuffer:
		_zbuffer->clear();
		break;
	case RenderMode::HierarchicalZBuffer:
		_quadTree->clearZBuffer();
		break;
	case RenderMode::OctreeHierarchicalZBuffer:
		_quadTree->clearZBuffer();
		break;
	}
}


/*
 * @brief assemble classified polygon table and classified edge table
 */
void ScanlineRenderer::_assembleRenderData(
	const Camera& camera,
	const std::vector<Model>& models,
	const glm::vec3& objectColor,
	const glm::vec3& lightColor,
	const glm::vec3& lightDirection) {
	// variables to be repeatly used in for loop
	const glm::mat4 projMat = camera.getProjectionMatrix();
	const glm::mat4 viewMat = camera.getViewMatrix();

	for (const auto& model : models) {
		glm::mat4 modelMat = model.getModelMatrix();
		const glm::mat4x4 mvp = projMat * viewMat * modelMat;
		const glm::mat3x3 normalMat = glm::mat3x3(glm::transpose(inverse(modelMat)));
		const glm::vec3 ambient = 0.1f * lightColor;

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		model.getFaces(vertices, indices);

		std::cout << "proj: \n"; Object3D::print(projMat); std::cout << std::endl;
		std::cout << "view: \n"; Object3D::print(viewMat); std::cout << std::endl;
		std::cout << "model: \n"; Object3D::print(modelMat); std::cout << std::endl;

		// for all triangles
		for (size_t i = 0; i < indices.size(); i += 3) {
			// get the raw data of a triangle
			Vertex tri[3] = {
				vertices[indices[i]],
				vertices[indices[i + 1]],
				vertices[indices[i + 2]],
			};

			// mvp transformation
			glm::vec4 v[3];
			for (int j = 0; j < 3; ++j) {
				v[j] = mvp * glm::vec4(tri[j].position, 1.0f);
			}

			// clip
			//std::vector<glm::vec4> points = _clipper.clip(v, 3);
			//if (points.size() < 3) {
			//	continue; // throw
			//}

			std::vector<glm::vec4> points = { v[0], v[1], v[2] };

			Polygon polygon;
			// to screen position
			std::vector<glm::vec3> screenPositions;
			for (const auto& point : points) {
				screenPositions.push_back(glm::vec3(
					_windowWidth * (point.x / point.w + 1.0f) / 2.0f,
					_windowHeight * (point.y / point.w + 1.0f) / 2.0f,
					point.z / point.w));
			}

			// get plane equation coefficients
			polygon.a = polygon.b = polygon.c = polygon.d = 0.0f;
			for (int j = 0; j < 3; ++j) {
				polygon.a += screenPositions[j].y
					* (screenPositions[(j + 1) % 3].z - screenPositions[(j + 2) % 3].z);
				polygon.b += screenPositions[j].z
					* (screenPositions[(j + 1) % 3].x - screenPositions[(j + 2) % 3].x);
				polygon.c += screenPositions[j].x
					* (screenPositions[(j + 1) % 3].y - screenPositions[(j + 2) % 3].y);
				polygon.d -= screenPositions[j].x * 
					(screenPositions[(j + 1) % 3].y * screenPositions[(j + 2) % 3].z 
					 - screenPositions[(j + 2) % 3].y * screenPositions[(j + 1) % 3].z);
			}

			// id of the triangle
			polygon.id = i;

			// number of scan line contained
			int minY = std::numeric_limits<int>::max();
			int maxY = std::numeric_limits<int>::min();
			for (const auto& pos : screenPositions) {
				minY = std::min(static_cast<int>(pos.y), minY);
				maxY = std::max(static_cast<int>(pos.y), maxY);
			}

			minY = std::max(0, minY);
			maxY = std::min(_windowHeight - 1, maxY);
			polygon.dy = maxY - minY;

			// get color(single color without interpolation)
			glm::vec3 norm = glm::normalize(normalMat * tri[0].normal);
			glm::vec3 diffuse = std::max(glm::dot(norm, lightDirection), 0.0f) * lightColor;
			polygon.color = (ambient + diffuse) * objectColor;

			_classifiedPolygonTable[maxY].push_back(polygon);

			Edge edge;
			// assemble edge of the polygon
			for (size_t j = 0; j < screenPositions.size(); ++j) {
				size_t m = j, n = (j + 1) % screenPositions.size();
				float maxY = -std::numeric_limits<float>::max();
				if (screenPositions[m].y > screenPositions[n].y) {
					maxY = screenPositions[m].y;
					edge.x = screenPositions[m].x;
					float dy = screenPositions[m].y - screenPositions[n].y;
					edge.dx = (screenPositions[n].x - screenPositions[m].x) / dy;
					edge.dy = static_cast<int>(dy);
				} else {
					maxY = screenPositions[n].y;
					edge.x = screenPositions[n].x;
					float dy = screenPositions[n].y - screenPositions[m].y;
					edge.dx = (screenPositions[m].x - screenPositions[n].x) / dy;
					edge.dy = static_cast<int>(dy);
				}

				edge.id = i;
				_classifiedEdgeTable[static_cast<int>(maxY)].push_front(edge);
			}
		}
	}

	std::cout << "poly list" << std::endl;
	for (auto polygons : _classifiedPolygonTable) {
		for (auto poly : polygons) {
			std::cout << "(" << poly.a << "," << poly.b << "," << poly.c << "," << poly.d << ")" << std::endl;
			std::cout << poly.id << std::endl;
			std::cout << poly.dy << std::endl;
			std::cout << "--------------------" << std::endl;
		}
	}

	std::cout << "edge list" << std::endl;
	for (auto edges : _classifiedEdgeTable) {
		for (auto edge : edges) {
			std::cout << edge.x << std::endl;
			std::cout << edge.dx << std::endl;
			std::cout << edge.dy << std::endl;
			std::cout << edge.id << std::endl;
			std::cout << "--------------------" << std::endl;
		}
	}
}


void ScanlineRenderer::_scan(Framebuffer& framebuffer) {
	// for each scan line
	for (int y = _windowHeight - 1; y >= 0; --y) {
		// for each polygon newly intersect with the scanline
		for (const auto& polygon : _classifiedPolygonTable[y]) {
			// add the new polygon in the scan line into the table
			_activePolygonTable.push_back(polygon);

			// find the edge belongs to polygon with polygon.id
			std::vector<Edge> edges;
			for (const auto& edge : _classifiedEdgeTable[y]) {
				if (edge.id == polygon.id && edge.dy > 0) { // drop 3 edge condition
					edges.push_back(edge);
				}
			}

			if (edges.size() != 2) {
				std::cout << edges.size() << std::endl;
				if (edges.size() == 1) {
					std::cout << edges[0].x << std::endl;
					std::cout << edges[0].dx << std::endl;
					std::cout << edges[0].dy << std::endl;
					std::cout << edges[0].id << std::endl;
				}
			}

			assert(edges.size() == 2);

			// so that edge[0] is at left side
			if (edges[0].x > edges[1].x || (edges[0].dx > edges[1].dx)) {
				std::swap(edges[0], edges[1]);
			}

			// add them into the active edge table
			ActiveEdgePair edgePair{};
			edgePair.xl = edges[0].x;
			edgePair.xr = edges[1].x;
			edgePair.dxl = edges[0].dx;
			edgePair.dxr = edges[1].dx;
			edgePair.dyl = edges[0].dy;
			edgePair.dyr = edges[1].dy;
			edgePair.zl = -(polygon.a * edges[0].x + polygon.b * y + polygon.d) / polygon.c;
			edgePair.dzx = -polygon.a / polygon.c;
			edgePair.dzy = polygon.b / polygon.c;
			edgePair.id = polygon.id;
			_activeEdgeTable.push_back(edgePair);
		}

		// for each edgePair
		for (auto& edgePair : _activeEdgeTable) {
			// get the polygon the edge belong to
			Polygon* pPolygon = nullptr;
			for (const auto& polygon : _activePolygonTable) {
				if (polygon.id == edgePair.id) {
					pPolygon = const_cast<Polygon*>(&polygon);
					break;
				}
			}

			if (pPolygon == nullptr) {
				std::cout << edgePair.id << std::endl;
			}

			assert(pPolygon != nullptr);

			// update framebuffer & zbuffer
			float zx = edgePair.zl;
			for (float x = edgePair.xl; x <= edgePair.xr; x += 1.0f) {
				if (_zbuffer->testAndSet(static_cast<float>(x), y, zx)) {
					framebuffer.setPixel(static_cast<float>(x), y, pPolygon->color);
				}
				zx += edgePair.dzx;
			}

			// update edge pair
			edgePair.xl += edgePair.dxl;
			edgePair.xr += edgePair.dxr;
			edgePair.zl += edgePair.dzy + edgePair.dzx * edgePair.dxl;

			edgePair.dyl -= 1;
			edgePair.dyr -= 1;

			// exchange outdated edges with new edges
			if (edgePair.dyl <= 0) { // left 
				for (const auto& edge : _classifiedEdgeTable[y]) {
					if (edge.id == edgePair.id && edge.dx >= 0.0f) {
						//??? do not need to change
						//edgePair.xl = edge.x;
						edgePair.dxl = edge.dx;
						edgePair.dyl = edge.dy;
					}
				}
			}

			if (edgePair.dyr <= 0) { // right
				for (const auto& edge : _classifiedEdgeTable[y]) {
					for (const auto& edge : _classifiedEdgeTable[y]) {
						if (edge.id == edgePair.id && edge.dx <= 0.0f) {
							//??? do not need to change
							//edgePair.xr = edge.x;
							edgePair.dxl = edge.dx;
							edgePair.dyl = edge.dy;
						}
					}
				}
			}
		}

		// update active polygon table
		for (auto it = _activePolygonTable.begin(); it != _activePolygonTable.end();) {
			it->dy -= 1;
			if (it->dy < 0) {
				_activePolygonTable.erase(it++);
			} else {
				++it;
			}
		}

	}
}


void ScanlineRenderer::_renderWithScanLineZBuffer() {

}


void ScanlineRenderer::_renderWithHierarchicalZBuffer(const Camera& camera) {
	_framebuffer.clear(_clearColor);
	_quadTree->clearZBuffer();

	glm::mat4x4 view = camera.getViewMatrix();
	glm::mat4x4 projection = camera.getProjectionMatrix();
	for (int i = 0; i < _triangles.size(); ++i) {
		_quadTree->handleTriangle(_triangles[i], view, projection);
	}

	_framebuffer.render();
}

void ScanlineRenderer::_renderWithOctreeHierarchicalZBuffer(const Camera& camera) {
	_framebuffer.clear(_clearColor);
	_quadTree->clearZBuffer();

	glm::mat4x4 view = camera.getViewMatrix();
	glm::mat4x4 projection = camera.getProjectionMatrix();

	std::stack<ptrOctreeZNode> stackNode;
	stackNode.push(new OctreeZNode{ 0.0f, _octree->getRoot() });
	while (!stackNode.empty()) {
		ptrOctreeZNode temp = stackNode.top();
		stackNode.pop();

		int screenX, screenY, screenRadius;
		glm::vec4 PVv = projection * view * glm::vec4{ temp->node->box->center, 1.0f };
		screenX = int((PVv.x / PVv.w + 1.0f) * _windowWidth / 2);
		screenY = int((PVv.y / PVv.w + 1.0f) * _windowHeight / 2);
		glm::vec3 vec = temp->node->box->center +
			glm::vec3(1.0f, 0.0f, 0.0f) * temp->node->box->halfSide * 1.73206f;
		glm::vec4 PVvec = projection * view * glm::vec4(vec, 1.0f);
		screenRadius = int((PVvec.x / PVvec.w + 1.0f) * _windowWidth / 2);

		QuadTreeNode* node = _quadTree->searchNode(screenX, screenY, screenRadius);
		if (node->z < temp->node->box->center.z + temp->node->box->halfSide * 1.73206f) {
			for (auto iter : temp->node->objects) {
				_quadTree->handleTriangle(*iter, view, projection);
			}
		}

		ptrOctreeZNode child[8];
		int count = 0;
		for (int i = 0; i < 8; ++i) {
			if (temp->node->childExists & (1 << i)) {
				uint32_t locCodeChild = (temp->node->locCode << 3) | i;
				OctreeNode* childNode = _octree->lookupNode(locCodeChild);
				glm::vec4 Vv = view * glm::vec4(childNode->box->center, 1.0f);
				child[count++] = new OctreeZNode{ Vv.z, childNode };
			}
		}
		std::sort(child, child + count, [](const ptrOctreeZNode& a, const ptrOctreeZNode& b) {
			return a->z > b->z;
			});
		while (count > 0) {
			stackNode.push(child[--count]);
		}
	}

	_framebuffer.render();
}