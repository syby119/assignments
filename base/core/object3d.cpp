/**
 * @file        base/core/object3d.cpp
 * @brief       base class for all object in the scene
 * @author      yy
 * @email       syby119@126.com
 * @date        2021/1/28
 * @copyright   MIT license
 */

#include <cassert>
#include <cmath>
#include <iostream>

#include "object3d.h"

/*
 * @brief constructor with specified name
 */
Object3D::Object3D(std::string name): _name(name) { }


/*
 * @brief get the name of the object
 * @return name of the object
 */
std::string Object3D::getName() const {
	return _name;
}


/*
 * @brief set the name of the object
 */
void Object3D::setName(std::string name) {
	_name = name;
}


/*
 * @brief get the parent of the object
 * @return the parent of the object or nullptr if no parent
 */
Object3D* Object3D::getParent() const {
	return _parent;
}


/*
 * @brief set the parent of the object
 * @param parent of the object, can be nullptr to detach from the parent
 * @todo maintain the world postion/rotation/scale of the object
 */
void Object3D::setParent(Object3D* parent, bool stayInWorld) {
	// check whether the object is already a child of the parent
	if (parent) {
		for (auto child : parent->_children) {
			if (child == this) {
				return;
			}
		}
	}
	
	if (stayInWorld) {
		assert("TODO: maintain the world postion/rotation/scale of the object");
	}

	// remove relation from the current parent
	if (_parent) {
		_parent->_children.remove(this);
	}
	
	// append this to the children of the parent
	if (parent) {
		parent->_children.push_back(this);
	}

	_parent = parent;
}


/*
 * @brief get the numbers of child of the object
 * @return the number of the child of the object
 */
uint32_t Object3D::childCount() const {
	return static_cast<uint32_t>(_children.size());
}


/*
 * @brief get child of the object by index
 * @return the pointer the the child object
 */
Object3D* Object3D::getChild(int index) const {
	auto it = _children.begin();
	std::advance(it, index);
	return *it;
}


/*
 * @brief get all children of the object
 * @return the list of 
 */
std::list<Object3D*> Object3D::getChildren() const {
	return _children;
}


/*
 * @brief get the local position of the object
 * @return the world/local position of the object
 */
Vector3<float> Object3D::getLocalPosition() const {
	return _localPosition;
}


/*
 * @brief set the local position of the object
 * @param postion local position corresponding to parent
 */
void Object3D::setLocalPosition(const Vector3<float>& position) {
	_localPosition = position;
}


/*
 * @brief get the world position of the object
 * @return the world position of the object
 */
Vector3<float> Object3D::getWorldPosition() const {
	// the result of Vector4<float> will be (x, y, z, 1.0f)
	// Vector3<float> use the x, y, z of the Vector4<float> to consturct self
	return getModelMatrix() * Vector4<float>(_localPosition, 1.0f);
}


/*
 * @brief set the world position of the object
 * @param position world position of 3 dimension
 */
void Object3D::setWorldPosition(const Vector3<float>& position) {
	_localPosition = Vector3<float>(getModelMatrixInverse() * Vector4<float>(position, 1.0f));
}


/*
 * @brief translate object with world/local translation
 * @param translation 3d vector of the movement in world/local space
 * @param space world/local space of the translation
 */
void Object3D::translate(const Vector3<float>& translation, enum Space space) {
	if (space == Space::Local) {
		_localPosition += translation;
	} else {
		_localPosition += Vector3<float>(getModelMatrixInverse() * Vector4<float>(translation, 0.0f));
	}
}


/*
 * @brief get the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
Quaternion<float> Object3D::getLocalRotation() const {
	return _localRotation;
}


/*
 * @brief set the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
void Object3D::setLocalRotation(const Quaternion<float>& rotation) {
	_localRotation = rotation;
}


/*
 * @brief get local euler angles of an object by extrinsic rotations
 * @param order rotate order of the euler angle
 * @return Vector3<float>(pitch, yaw, roll)
 *         pitch vec3.x rotate angle around x-axis
 *         yaw   vec3.y rotate angle around y-axis
 *         roll  vec3.z rotate angle around z-axis
 * @todo overcome singularity(atan2) and precision problems
 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
 */
Vector3<float> Object3D::getLocalEulerAngles(enum RotateOrder order) const {
	return quaternionToEulerAngles(_localRotation, order);
}


/*
 * @brief set local euler angles of an object by extrinsic rotations
 * @param Vector3<float>(pitch, yaw, roll)
 *         pitch vec3.x rotate angle around x-axis
 *         yaw   vec3.y rotate angle around y-axis
 *         roll  vec3.z rotate angle around z-axis
 * @param order rotate order of the euler angle
 * @todo overcome singularity(atan2) and precision problems
 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
 */
void Object3D::setLocalEulerAngles(const Vector3<float>& eulerAngles, enum RotateOrder order) {
	Quaternion<float> q = eulerAnglesToQuaternion(eulerAngles, order);
	_localRotation = q * _localRotation;
}


/*
 * @brief rotate by axis and angle in local space
 * @param axis normalized rotate axis
 * @param angle rotate angle in radians
 * @todo add world space rotate support
 */
void Object3D::rotate(const Vector3<float>& axis, float angle) {
	_localRotation = glm::angleAxis(angle, axis) * _localRotation;
}


/*
 * @brief rotate by axis and angle in local space
 * @param eulerAngles in Vector3<float>(pitch, yaw, roll)
 *						 pitch vec3.x rotate angle around x-axis
 *                       yaw   vec3.y rotate angle around y-axis
 *                       roll  vec3.z rotate angle around z-axis
 * @param order rotate order
 * @todo add world space rotate support
 */
void Object3D::rotate(const Vector3<float>& eulerAngles, const enum RotateOrder order) {
	_localRotation = eulerAnglesToQuaternion(eulerAngles, order) * _localRotation;
}


/*
 * @brief get local scale of the object
 * @return the local scale of the object in Vector3<float>
 */
Vector3<float> Object3D::getLocalScale() const {
		return _localScale;
}


/*
 * @brief set local scale of the object
 * @param scale local scale of the object
 */
void Object3D::setLocalScale(const Vector3<float>& scale) {
	_localScale = scale;
}


/*
 * @brief get model matrix of the object
 * @return model matrix to transform the object from local to world space
 */
Matrix4x4<float> Object3D::getModelMatrix() const {
	Matrix4x4<float> m(1.0f);

	for (Object3D* obj = const_cast<Object3D*>(this); obj; obj = obj->_parent) {
		m = glm::scale(m, obj->_localScale);
		m = glm::mat4_cast(obj->_localRotation) * m;
		m = glm::translate(m, obj->_localPosition);
	}
	
	return m;
}


/*
 * @brief get model matrix inverse of the object
 * @return model matrix inverse to transform the object from world to local space
 */
glm::mat4x4 Object3D::getModelMatrixInverse() const {
	return glm::inverse(getModelMatrix());
}


/*
 * @brief print local position info
 */
void Object3D::printPosition() const {
	std::cout << _name << ": position";
	print(_localPosition);
	std::cout << std::endl;
}


/*
 * @brief print local rotation info
 */
void Object3D::printRotation() const {
	std::cout << _name << ": rotation";
	print(_localRotation);
	std::cout << std::endl;
}


/*
 * @brief print local scale info
 */
void Object3D::printScale() const {
	std::cout << _name << ": scale";
	print(_localScale);
	std::cout << std::endl;
}
