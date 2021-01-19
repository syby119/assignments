#include <iostream>
#include "scanline_renderer.h"
#include <cstdio>


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
		_clearRenderData();
		framebuffer.clear(_clearColor);

		_assembleRenderData(camera, models, objectColor, lightColor, lightDirection);

		_scan(framebuffer);

		framebuffer.render();
	} else if (_renderMode == RenderMode::HierarchicalZBuffer) {
		_renderWithHierarchicalZBuffer(camera, objectColor, lightColor, lightDirection);
	} else {
		_renderWithOctreeHierarchicalZBuffer(camera, objectColor, lightColor, lightDirection);
	}
}


enum ScanlineRenderer::RenderMode ScanlineRenderer::getRenderMode() const {
	return _renderMode;
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
		//_quadTree->clear();
		break;
	case RenderMode::OctreeHierarchicalZBuffer:
		//_quadTree->clear();
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

		// for all triangles
		for (size_t i = 0; i < indices.size(); i += 3) {
			// get the raw data of a triangle
			Vertex tri[3] = {
				vertices[indices[i]],
				vertices[indices[i + 1]],
				vertices[indices[i + 2]],
			};

			//if (i == 3252 || i == 58521) {
			//	std::cout << "triangle " << i << std::endl;
			//	std::cout << "raw data" << std::endl;
			//	Object3D::print(tri[0].position); std::cout << "\n";
			//	Object3D::print(tri[1].position); std::cout << "\n";
			//	Object3D::print(tri[2].position); std::cout << "\n";
			//}

			// mvp transformation
			glm::vec4 v[3];
			for (int j = 0; j < 3; ++j) {
				v[j] = mvp * glm::vec4(tri[j].position, 1.0f);
			}

			//if (i == 3252 || i == 58521) {
			//	std::cout << "triangle " << i << std::endl;
			//	std::cout << "mvp transformed data" << std::endl;
			//	Object3D::print(v[0]); std::cout << "\n";
			//	Object3D::print(v[1]); std::cout << "\n";
			//	Object3D::print(v[2]); std::cout << "\n";
			//}


			// clip
			//std::vector<glm::vec4> points = _clipper.clip(v, 3);
			//if (points.size() < 3) {
			//	continue; // throw
			//}

			if (v[0].w <= 0 && v[1].w <= 0 && v[2].w <= 0) {
				continue;
			}
			else if (v[0].w > 0 && v[1].w > 0 && v[2].w > 0) {
				if (v[0].w < v[0].z && v[1].w < v[1].z && v[2].w < v[2].z) {
					continue;
				} else if (-v[0].w > v[0].z && -v[1].w > v[1].z && -v[2].w > v[2].z) {
					continue;
				}
			}

			std::vector<glm::vec4> points = { v[0], v[1], v[2] };

			Polygon polygon;
			// to screen position
			int screenX[3], screenY[3];
			double screenZ[3];

			for (int j = 0; j < 3; ++j) {
				screenX[j] = _windowWidth *  (points[j].x / points[j].w + 1.0f) / 2.0f;
				screenY[j] = _windowHeight * (points[j].y / points[j].w + 1.0f) / 2.0f,
				screenZ[j] = points[j].z / points[j].w;
			}


			//if (i == 3252 || i == 58521) {
			//	std::cout << "polygon " << i << std::endl;
			//	std::cout << "screen data" << std::endl;
			//	std::cout << "\n";
			//	printf("%d, %d, %f\n", screenX[0], screenY[0], screenZ[0]);
			//	printf("%d, %d, %f\n", screenX[1], screenY[1], screenZ[1]);
			//	printf("%d, %d, %f\n", screenX[2], screenY[2], screenZ[2]);
			//}

			//for (int j = 0; j < 3; ++j) {
			//	Object3D::print(tri[j].position);
			//	std::cout << " -> ";  Object3D::print(points[j]);
			//	std::cout << " -> "; Object3D::print(screenPositions[j]);
			//	std::cout << "\n";
			//}

			// get plane equation coefficients
			polygon.a = polygon.b = polygon.c = polygon.d = 0.0f;
			for (int j = 0; j < 3; ++j) {
				polygon.a += screenY[j] * (screenZ[(j + 1) % 3] - screenZ[(j + 2) % 3]);
				polygon.b += screenZ[j] * (screenX[(j + 1) % 3] - screenX[(j + 2) % 3]);
				polygon.c += screenX[j] * (screenY[(j + 1) % 3] - screenY[(j + 2) % 3]);
				polygon.d -= screenX[j] * (screenY[(j + 1) % 3] * screenZ[(j + 2) % 3] - 
					                       screenY[(j + 2) % 3] * screenZ[(j + 1) % 3]);
			}

			// id of the triangle
			polygon.id = static_cast<int>(i);

			// number of scan line contained
			int minY = std::numeric_limits<int>::max();
			int maxY = std::numeric_limits<int>::min();
			for (int i = 0; i < 3; ++i) {
				minY = std::min(screenY[i], minY);
				maxY = std::max(screenY[i], maxY);
			}

			minY = std::max(0, minY);
			if (maxY < 0) {
				continue;
			} else if (maxY >= _windowHeight) {
				maxY = _windowHeight - 1;
			}

			if (minY == maxY) {
				continue;
			}

			polygon.dy = maxY - minY;

			// get color(single color without interpolation)
			glm::vec3 norm = glm::normalize(normalMat * tri[0].normal);
			glm::vec3 diffuse = std::max(glm::dot(norm, lightDirection), 0.0f) * lightColor;
			polygon.color = (ambient + diffuse) * objectColor;
			//if ((normalMat * tri[0].normal).z < 0) {
			//	polygon.color = glm::vec3(1, 0, 0);
			//}

			//if (i == 3054) {
			//	Object3D::print(polygon.color); std::cout << std::endl;
			//	polygon.color = glm::vec3(1.0f, 0.0f, 0.0f);
			//}

			_classifiedPolygonTable[maxY].push_back(polygon);

			//if (polygon.id == 3252 || polygon.id == 58521) {
			//	std::cout << "polygon id: " << polygon.id << std::endl;
			//	_print(polygon);
			//}

			Edge edge;
			// assemble edge of the polygon
			for (int j = 0; j < 3; ++j) {
				int m = j, n = (j + 1) % 3;
				// make sure that the screenY[m] < screenY[n]  
				if (screenY[m] == screenY[n]) {
					if (screenX[m] > screenX[n]) {
						std::swap(m, n);
					}
				} else {
					if (screenY[m] > screenY[n]) {
						std::swap(m, n);
					}
				}

				if (screenY[n] < 0 || screenY[m] > _windowHeight) {
					continue;
				}

				edge.x = screenX[n];
				edge.dx =  1.0f * (screenX[m] - screenX[n]) / (screenY[n] - screenY[m] + 0.000001);
				edge.dy = std::clamp(screenY[n], 0, _windowHeight - 1) -
						  std::clamp(screenY[m], 0, _windowHeight - 1);
				edge.id = static_cast<int>(i);

				//if (edge.id == 3252 || edge.id == 58521) {
				//	std::cout << "edge id: " << edge.id << std::endl;
				//	_print(edge);
				//}

				_classifiedEdgeTable[std::clamp(screenY[n], 0, _windowHeight - 1)].push_front(edge);
			}
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

				if (edge.dy < 0) {
					std::cout << "error: edge.dy < 0" << std::endl;
				}
			}

			if (edges.size() != 2) {
				std::cout << "error: edges.size < 2 , true size " << edges.size() << std::endl;
				continue;
			}
			//assert(edges.size() == 2);

			int left = 0, right = 1;
			if (edges[0].x > edges[1].x ||
				(edges[0].x == edges[1].x && edges[0].dx > edges[1].dx)) {
				left = 1, right = 0;
			}

			if (edges[0].x == edges[1].x && edges[0].dx == edges[1].dx) {
				continue;
			}

			// add them into the active edge table
			ActiveEdgePair edgePair{};
			edgePair.xl = edges[left].x;
			edgePair.xr = edges[right].x;
			edgePair.dxl = edges[left].dx;
			edgePair.dxr = edges[right].dx;
			edgePair.dyl = edges[left].dy;
			edgePair.dyr = edges[right].dy;
			edgePair.zl = -(polygon.a * (int)(edges[left].x) + polygon.b * y + polygon.d) / polygon.c;
			edgePair.dzx = -polygon.a / polygon.c;
			edgePair.dzy = polygon.b / polygon.c;
			edgePair.id = polygon.id;

			_activeEdgeTable.push_back(edgePair);
		}

		// for each edgePair
		for (auto edgePairIt = _activeEdgeTable.begin(); edgePairIt != _activeEdgeTable.end();) {
			// get the polygon the edge belong to
			Polygon* pPolygon = _findActivePolygon(edgePairIt->id);

			assert(pPolygon != nullptr);

			// update framebuffer & zbuffer
			float zx = edgePairIt->zl;
			for (int x = edgePairIt->xl; x < (int)edgePairIt->xr; ++x) {
				if (x >= 0 && x < _windowWidth) {
					if (_zbuffer->testAndSet(x, y, zx)) {
						framebuffer.setPixel(x, y, pPolygon->color);
					}
				}
				zx += edgePairIt->dzx;
			}

			// update edge pair
			edgePairIt->xl += edgePairIt->dxl;
			edgePairIt->xr += edgePairIt->dxr;
			edgePairIt->zl += edgePairIt->dzy + edgePairIt->dzx * edgePairIt->dxl;

			edgePairIt->dyl -= 1;
			edgePairIt->dyr -= 1;

			// exchange outdated edges with new edges in the same polygon
			if ((edgePairIt->dyl <= 0 || edgePairIt->dyr <= 0) && y >= 1) {
				std::vector<Edge> candidateEdges;
				for (const auto& edge : _classifiedEdgeTable[static_cast<size_t>(y) - 1]) {
					if (edge.id == edgePairIt->id) {
						candidateEdges.push_back(edge);
						if ((edgePairIt->dyl > 0 || edgePairIt->dyr > 0) || candidateEdges.size() > 1) {
							break;
						}
					}
				}

				if (candidateEdges.empty()) {
					_activeEdgeTable.erase(edgePairIt++);
					continue;
				}

				if (candidateEdges.size() == 1) {
					if (edgePairIt->dyl <= 0) {
						Polygon* pPolygon = _findActivePolygon(edgePairIt->id);
						assert(pPolygon != nullptr);

						edgePairIt->xl = candidateEdges[0].x;
						edgePairIt->dxl = candidateEdges[0].dx;
						edgePairIt->dyl = candidateEdges[0].dy;
						edgePairIt->zl = -(pPolygon->a * candidateEdges[0].x + pPolygon->b * y + pPolygon->d) / pPolygon->c;
					}
					else if (edgePairIt->dyr <= 0) {
						edgePairIt->xr = candidateEdges[0].x;
						edgePairIt->dxr = candidateEdges[0].dx;
						edgePairIt->dyr = candidateEdges[0].dy;
					}
				}

				if (edgePairIt->xl > edgePairIt->xr) {
					edgePairIt->xl = edgePairIt->xr;
					//std::cout << edgePairIt->xl - edgePairIt->xr << std::endl;
					//std::cout << __LINE__ << " error" << std::endl;
				}
			}

			++edgePairIt;
		}

		// update active polygon table
		for (auto it = _activePolygonTable.begin(); it != _activePolygonTable.end();) {
			it->dy -= 1;
			if (it->dy < 0) {
				_activePolygonTable.erase(it++);
			}
			else {
				++it;
			}
		}
	}
}



Polygon* ScanlineRenderer::_findActivePolygon(int id) {
	for (auto& polygon : _activePolygonTable) {
		if (polygon.id == id) {
			return &polygon;
		}
	}

	return nullptr;
}


void ScanlineRenderer::_renderWithScanLineZBuffer() {

}


void ScanlineRenderer::_renderWithHierarchicalZBuffer(
	const Camera& camera, 
	const glm::vec3& objectColor,
	const glm::vec3& lightColor,
	const glm::vec3& lightDirection) {
	_framebuffer.clear(_clearColor);
	_quadTree->clear();

	glm::mat4x4 model = glm::mat4x4(1.0f);
	glm::mat4x4 view = camera.getViewMatrix();
	glm::mat4x4 projection = camera.getProjectionMatrix();

	for (int i = 0; i < _triangles.size(); ++i) {
		//if (i == 3054 / 3) {
			_quadTree->handleTriangle(_triangles[i], 
				model, view, projection, objectColor, lightColor, lightDirection);
		//}
	}

	_framebuffer.render();
}


void ScanlineRenderer::_renderWithOctreeHierarchicalZBuffer(
	const Camera& camera, 
	const glm::vec3& objectColor,
	const glm::vec3& lightColor,
	const glm::vec3& lightDirection) {

	_framebuffer.clear(_clearColor);
	_quadTree->clear();

	glm::mat4x4 model = glm::mat4x4(1.0f);
	glm::mat4x4 view = camera.getViewMatrix();
	glm::mat4x4 projection = camera.getProjectionMatrix();

	std::stack<ptrOctreeZNode> stackNode;
	stackNode.push(new OctreeZNode{ 1.0f, _octree->getRoot() });
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
		if (node->z > temp->node->box->center.z + temp->node->box->halfSide * 1.73206f) {
			for (auto iter : temp->node->objects) {
				_quadTree->handleTriangle(*iter, model, view, projection, 
					objectColor, lightColor, lightDirection);
			}
		}

		OctreeZNode* child[8];
		int count = 0;
		for (int i = 0; i < 8; ++i) {
			if (temp->node->childExists & (1 << i)) {
				uint32_t locCodeChild = (temp->node->locCode << 3) | i;
				OctreeNode* childNode = _octree->lookupNode(locCodeChild);
				glm::vec4 PVv = projection * view * glm::vec4(childNode->box->center, 1.0f);
				child[count++] = new OctreeZNode{ PVv.z, childNode };
			}
		}
		std::sort(child, child + count, [](const ptrOctreeZNode& a, const ptrOctreeZNode& b) {
			return a->z < b->z;
			});
		while (count > 0) {
			stackNode.push(child[--count]);
		}
	}

	_framebuffer.render();
}


void ScanlineRenderer::_print(const Polygon& polygon) {
	std::cout << "Polygon.a, b, c, d [" 
		<< polygon.a << "," << polygon.b << "," << polygon.c << "," << polygon.d << "]\n";
	std::cout << "Polygon.id " << polygon.id << "\n";
	std::cout << "Polygon.dy " << polygon.dy << "\n";
	std::cout << "-----------------------" << std::endl;
}

void ScanlineRenderer::_print(const Edge& edge) {
	std::cout << "Edge.x " << edge.x << "\n";
	std::cout << "Edge.dx " << edge.dx << "\n";
	std::cout << "Edge.dy " << edge.dy << "\n";
	std::cout << "Edge.id " << edge.id << "\n";
	std::cout << "-----------------------" << std::endl;
}

void ScanlineRenderer::_print(const ActiveEdgePair& edgePair) {
	std::cout << "ActiveEdgePair.[xl, xr] " << "[" << edgePair.xl << ", " << edgePair.xr << "]\n";
	std::cout << "ActiveEdgePair.dxl " << edgePair.dxl << "\n";
	std::cout << "ActiveEdgePair.dxr " << edgePair.dxr << "\n";
	std::cout << "ActiveEdgePair.dyl " << edgePair.dyl << "\n";
	std::cout << "ActiveEdgePair.dyr " << edgePair.dyr << "\n";
	std::cout << "ActiveEdgePair.zl " << edgePair.zl << "\n";
	std::cout << "ActiveEdgePair.dzx " << edgePair.dzx << "\n";
	std::cout << "ActiveEdgePair.dzy " << edgePair.dzy << "\n";
	std::cout << "ActiveEdgePair.id " << edgePair.id << "\n";
	std::cout << "-----------------------" << std::endl;
}