#pragma once

#include <vector>
#include <glm/vec4.hpp>

class Clipper {
public:
	/*
	 * @brief clip a triangle to polygon in homogeneous normalized coordinates(mvp * v)
	 * @return vector of homogeneous coordinates points representing a polygon
	 */
	std::vector<glm::vec4> clip(const glm::vec4* v, int vertexCount);

	enum class ClipPlane {
		Left, Right, Bottom, Top, Near, Far
	};
	
private:
	void _clipPoint(const glm::vec4& v, enum ClipPlane plane,
					std::vector<glm::vec4>& result, glm::vec4* first[], glm::vec4* s);

	/*
	 * @brief auxliary function for clip
	 * @return true for inside, false otherwise
	 */
	void _closeClip(std::vector<glm::vec4>& result, glm::vec4* first[], glm::vec4* s);


	/*
	 * @brief check if a point is inside the plane of normalized coordinate box
	 */
	bool _inside(const glm::vec4& v, enum ClipPlane plane);


	/*
	 * @brief check if two points are at the different side of the clip plane
	 */
	bool _cross(const glm::vec4& v1, const glm::vec4& v2, enum ClipPlane plane);

	/*
	 * @brief get the intersect point of two points with a clip plane
	 * @return interpolate point
	 */
	glm::vec4 _intersect(const glm::vec4& v1, const glm::vec4& v2, enum ClipPlane plane);


	/*
	 * @brief get the next clip plane
	 * @return the next clip plane of the current clip plane
	 */
	enum ClipPlane _nextPlane(enum ClipPlane plane) const ;

	/*
	 * @brief get the index of the clip plane
	 * @return the index of the clip plane
	 */
	int _planeIndex(enum ClipPlane plane) const;


	enum ClipPlane _getPlane(int index) const;
};