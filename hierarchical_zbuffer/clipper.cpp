#include "clipper.h"
#include <iostream>


/*
 * @brief clip a triangle to polygon in homogeneous normalized coordinates(mvp * v)
 * @detail Sutherland-Hodgman algorithm
 * @param v vertex array of a convex polygon
 * @param vertexCount number of the vertices 
 * @return vector of homogeneous coordinates points representing a polygon
 */
std::vector<glm::vec4> Clipper::clip(const glm::vec4* v, int vertexCount) {
	glm::vec4* first[6] = { nullptr };
	glm::vec4 s[6];

	std::vector<glm::vec4> result;
	for (int i = 0; i < vertexCount; ++i) {
		_clipPoint(v[i], _getPlane(0), result, first, s);
	}

	_closeClip(result, first, s);

	return result;
}


/*
 * @brief clip a point in homogeneous normalized coordinates(mvp * v) with a clip plane
 * @param homogeneous normalized coordinats of a convex polygon
 * @return vector of homogeneous coordinates points representing a polygon
 */
void Clipper::_clipPoint(const glm::vec4& v, enum ClipPlane plane,
	std::vector<glm::vec4>& result, glm::vec4* first[], glm::vec4* s) {
	int index = _planeIndex(plane);
	if (first[index] == nullptr) {
		std::cout << "first: " << index << std::endl;
		first[index] = const_cast<glm::vec4*>(&v);
	} else {
		if (_cross(v, s[index], plane)) {
			glm::vec4 u = _intersect(v, s[index], plane);
			if (index < 5) {
				_clipPoint(u, _nextPlane(plane), result, first, s); 
			} else {
				result.push_back(u);
			}
		}
	}

	s[index] = v;
	if (_inside(v, plane)) {
		if (index < 5) {
			_clipPoint(v, _nextPlane(plane), result, first, s);
		} else {
			result.push_back(v);
		}
	}
}

/*
 * @brief finish the clip
 * @param result result to store candidates
 * @param first see function clip
 * @param s see function clip
 * @return vector of homogeneous coordinates points representing a polygon
 */
void Clipper::_closeClip(std::vector<glm::vec4>& result, glm::vec4* first[], glm::vec4* s) {
	for (int index = 0; index < 6; ++index) {
		enum ClipPlane plane = _getPlane(index);
		if (_cross(s[index], *first[index], plane)) {
			glm::vec4 v = _intersect(s[index], *first[index], plane);
			if (_planeIndex(plane) < 5) {
				_clipPoint(v, _nextPlane(plane), result, first, s);
			} else {
				result.push_back(v);
			}
		}
	}
}


/*
 * @brief check if a point is inside the plane of normalized coordinate box
 * @param v 4-d point
 * @param plane clip plane
 * @return true for inside, false otherwise
 */
bool Clipper::_inside(const glm::vec4& v, enum ClipPlane plane) {
	switch (plane) {
	case ClipPlane::Left:
		if ((v.w > 0.0f && v.x >= -v.w) || (v.w < 0.0f && v.x <= -v.w))
			return true;
		break;
	case ClipPlane::Right:
		if ((v.w > 0.0f && v.x <= v.w) || (v.w < 0.0f && v.x >= v.w))
			return true;
		break;
	case ClipPlane::Bottom:
		if ((v.w > 0.0f && v.y >= -v.w) || (v.w < 0.0f && v.y <= -v.w))
			return true;
		break;
	case ClipPlane::Top:
		if ((v.w > 0.0f && v.y <= v.w) || (v.w < 0.0f && v.y >= v.w))
			return true;
		break;
	case ClipPlane::Near:
		if (v.w >= 0.0f && v.z >= -v.w)
			return true;
		break;
	case ClipPlane::Far:
		if (v.w <= 0.0f || (v.w > 0.0f && v.z < v.w))
			return true;
		break;
	}

	return false;
}


/*
 * @brief check if two points are at the different side of the clip plane
 * @param v1 4-d point
 * @param v2 4-d point
 * @param plane clip plane
 * @return true for the edge cross the plane, false otherwise
 */
bool Clipper::_cross(const glm::vec4& v1, const glm::vec4& v2, enum ClipPlane plane) {
	if (_inside(v1, plane) == _inside(v2, plane)) {
		return false;
	} else if (v1.w <= 0 || v2.w <= 0) {
		return false;
	}

	return true;
}


/*
 * @brief get the intersect point of two points with a clip plane
 * @param v1 first 4d-point
 * @param v2 second 4d-point
 * @param plane clip plane
 * @return interpolate point
 * @note must be used if _cross is true
 */
glm::vec4 Clipper::_intersect(const glm::vec4& v1, const glm::vec4& v2, enum ClipPlane plane) {
	const glm::vec4 dv = v2 - v1;
	float t = 0.0f;

	switch (plane) {
	case ClipPlane::Left:
		t = -(v1.x + v1.w) / (dv.x + dv.w);
		break;
	case ClipPlane::Right:
		t = (v1.x + v1.w) / (dv.x + dv.w);
		break;
	case ClipPlane::Bottom:
		t = -(v1.y + v1.w) / (dv.y + dv.w);
		break;
	case ClipPlane::Top:
		t = (v1.y + v1.w) / (dv.y + dv.w);
		break;
	case ClipPlane::Near:
		t = -(v1.z + v1.w) / (dv.z + dv.w);
		break;
	case ClipPlane::Far:
		t = (v1.z + v1.w) / (dv.z + dv.w);
		break;
	}

	std::cout << "t:" << t << std::endl;

	return v1 + t * dv;
}


/*
 * @brief get the next clip plane
 * @param plane current clip plane
 * @return the next clip plane of the current clip plane
 */
enum Clipper::ClipPlane Clipper::_nextPlane(enum ClipPlane plane) const {
	return static_cast<enum ClipPlane>(static_cast<int>(plane) + 1);
}


/*
 * @brief get the index of the clip plane
 * @param plane the clip plane
 * @return the index of the clip plane
 */
int Clipper::_planeIndex(enum ClipPlane plane) const {
	return static_cast<int>(plane);
}


enum Clipper::ClipPlane Clipper::_getPlane(int index) const {
	return static_cast<enum ClipPlane>(index);
}