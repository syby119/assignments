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
#include "../math/print_info.hpp"
#include "../math/rotation_cast.hpp"
#include "../math/transformation.hpp"

/*
 * @brief constructor with specified name
 */
Object3D::Object3D(const std::string& name): _name(name) { }


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
void Object3D::setName(const std::string& name) {
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
 * @return the local position of the object
 */
gtm::Vector3<float> Object3D::getLocalPosition() const {
	return _position;
}


/*
 * @brief set the local position of the object
 * @param postion local position corresponding to parent
 */
void Object3D::setLocalPosition(const gtm::Vector3<float>& position) {
	_position = position;
}


/*
 * @brief get the world position of the object
 * @return the world position of the object
 */
gtm::Vector3<float> Object3D::getWorldPosition() const {
	return gtm::point_cast(getModelMatrix() * gtm::point_cast(_position));
}


/*
 * @brief set the world position of the object
 * @param position world position of 3 dimension
 */
void Object3D::setWorldPosition(const gtm::Vector3<float>& position) {
	_position = gtm::point_cast(getModelMatrixInverse() * gtm::point_cast(_position));
}


/*
 * @brief translate object with world/local translation
 * @param translation 3d vector of the movement in world/local space
 * @param space world/local space of the translation
 */
void Object3D::translate(const gtm::Vector3<float>& translation, enum Space space) {
	if (space == Space::Local) {
		_position += translation;
	} else {
		_position += gtm::point_cast(getModelMatrixInverse() * gtm::point_cast(translation)); ???
	}
}


/*
 * @brief get the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
gtm::Quaternion<float> Object3D::getLocalRotation() const {
	return _rotation;
}


/*
 * @brief set the local rotation of the object in quaternion
 * @return the local rotation of the object in quaternion
 */
void Object3D::setLocalRotation(const gtm::Quaternion<float>& rotation) {
	_rotation = rotation;
}


/*
 * @brief get local euler angles of an object by intrinsic rotations
 * @return the local euler angle transformed from quaternion
 */
gtm::EulerAngle<float> Object3D::getLocalEulerAngle(enum gtm::RotateOrder order) const {
	return gtm::euler_cast(_rotation, order);
}


/*
 * @brief set local euler angles of an object by intrinsic rotations
 * @param order rotate order of the euler angle
 * @todo overcome singularity(atan2) and precision problems
 * @see https://blog.csdn.net/hzwwpgmwy/article/details/101547949
 */
void Object3D::setLocalEulerAngle(const gtm::EulerAngle<float>& eulerAngles) {
	gtm::Quaternion<float> q = gtm::quat_cast(eulerAngles);
	_rotation = q * _rotation;
}


/*
 * @brief rotate by axis and angle in local space
 * @param axis normalized rotate axis
 * @param angle rotate angle in radians
 * @todo add world space rotate support
 */
void Object3D::rotate(const Vector3<float>& axis, float angle) {
	_rotation = glm::angleAxis(angle, axis) * _localRotation;
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
void Object3D::rotate(const gtm::Vector3<float>& eulerAngle) {
	_rotation = eulerAnglesToQuaternion(eulerAngle, order) * _localRotation;
}


/*
 * @brief get local scale of the object
 * @return the local scale of the object in Vector3<float>
 */
gtm::Vector3<float> Object3D::getLocalScale() const {
		return _localScale;
}


/*
 * @brief set local scale of the object
 * @param scale local scale of the object
 */
void Object3D::setLocalScale(const gtm::Vector3<float>& scale) {
	_scale = scale;
}


/*
 * @brief get model matrix of the object
 * @return model matrix to transform the object from local to world space
 */
gtm::Matrix4x4<float> Object3D::getModelMatrix() const {
	gtm::Matrix4x4<float> m(1.0f);

	for (Object3D* obj = const_cast<Object3D*>(this); obj; obj = obj->_parent) {
		m = gtm::translate(obj->_position) * \
			gtm::mat4_cast(obj->_rotation) * \
			gtm::scale(obj->_scale) * m;
	}
	
	return m;
}


/*
 * @brief get model matrix inverse of the object
 * @return model matrix inverse to transform the object from world to local space
 */
gtm::Matrix4x4<float> Object3D::getModelMatrixInverse() const {
	return gtm::inverse(getModelMatrix());
}


/*
 * @brief print local position info
 */
void Object3D::printPosition() const {
	std::cout << _name << ": position";
	gtm::print(_localPosition);
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
