/**
 * @file        base/core/object3d.h
 * @brief       base class for all object in the scene
 * @author      yy
 * @email       syby119@126.com
 * @date        2021/1/28
 * @copyright   MIT license
 */

#pragma once

#include <list>
#include <string>

#include "../math/matrix3x3.hpp"
#include "../math/matrix4x4.hpp"
#include "../math/quaternion.hpp"
#include "../math/euler_angle.hpp"

class Object3D {
public:
	/** 
	 * @brief enum class for coordinate system type
	 */
	enum class Space {
		Local, World
	};

	/**
	 * @brief default constructor
	 */
	Object3D() = default;

	/**
	 * @brief constructor with specified name
	 */
	Object3D(const std::string& name);
	
	/**
     * @brief default destructor
     */
	~Object3D() = default;

	/**
	 * @brief get the name of the object
	 */
	std::string getName() const;

	/**
	 * @brief set the name of the object
     */
	void setName(const std::string& name);

	/**
	 * @brief get the parent of the object, if no parent return nullptr
	 */
	Object3D* getParent() const;

	/**
	 * @brief set the new parent of the object
	 * @todo maintain the world postion/rotation/scale of the object
	 */
	void setParent(Object3D* parent, bool stayInWorld = false);

	/**
	 * @brief get the numbers of child of the object
	 */
	uint32_t childCount() const;

	/**
	 * @brief get child of the object by index
	 */
	Object3D* getChild(int index) const;

	/**
	 * @brief get all children of the object
     */
	std::list<Object3D*> getChildren() const;

	/**
	 * @brief get the local position of the object
	 * @return the world/local position of the object
	 */
	gtm::Vector3<float> getLocalPosition() const;

	/**
	 * @brief set the local position of the object
	 */
	void setLocalPosition(const gtm::Vector3<float>& position);

	/**
	 * @brief get the world position of the object
	 */
	gtm::Vector3<float> getWorldPosition() const;

	/**
	 * @brief set the world position of the object
	 */
	void setWorldPosition(const gtm::Vector3<float>& position);

	/**
	 * @brief translate object with world/local translation
	 */
	void translate(const gtm::Vector3<float>& translation, enum Space space);

	/**
     * @brief get the local rotation of the object in quaternion
     */
	gtm::Quaternion<float> getLocalRotation() const;

	/**
	 * @brief set the local rotation of the object in quaternion
	 */
	void setLocalRotation(const gtm::Quaternion<float>& rotation);

	/**
	 * @brief get local euler angles of an object by intrinsic rotations
	 */
	gtm::EulerAngle<float> getLocalEulerAngle(enum gtm::RotateOrder order) const;

	/**
	 * @brief set local euler angles of an object by extrinsic rotations
	 */
	void setLocalEulerAngle(const gtm::EulerAngle<float>& e);

	/**
	 * @brief rotate by normalized axis and angle in radians in local space
	 * @todo add world space rotate support
	 */
	void rotate(const gtm::Vector3<float>& axis, float angle);

	/**
	 * @brief rotate by euler angles and specified rotate order in local space
	 * @todo add world space rotate support
	 */
	void rotate(const gtm::Vector3<float>& eulerAngles);


	/**
	 * todo: rotateAround
	 */
	void rotateAround(
		const gtm::Vector3<float>& origin, const gtm::Vector3<float>& axis, float angle);

	/**
	 * @brief get local scale of the object
	 * @return local scale of the object in 3 dimensions
	 */
	gtm::Vector3<float> getLocalScale() const ;

	/**
     * @brief set local scale of the object
     */
	void setLocalScale(const gtm::Vector3<float>& scale);

	/**
	 * @brief get model matrix of the object
	 */
	gtm::Matrix4x4<float> getModelMatrix() const;

	/**
	 * @brief get model matrix inverse of the object
	 */
	gtm::Matrix4x4<float> getModelMatrixInverse() const;

	/**
	 * @brief print local position info
	 */
	void printPosition() const;

	/**
	 * @brief print local rotation info
	 */
	void printRotation() const ;

	/**
	 * @brief print local scale info
	 */
	void printScale() const;

protected:
	/** name of the object */
	std::string _name;
	/** local position of the object */
	gtm::Vector3<float> _position{ 0.0f, 0.0f, 0.0f };
	/** local rotation of the object */
	gtm::Quaternion<float> _rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	/** local scale of the object */
	gtm::Vector3<float> _scale{ 1.0f, 1.0f, 1.0f };
	/** relationship:parent */
	Object3D* _parent = nullptr;
	/** relationship children */
	std::list<Object3D*> _children;

private:
	/** universal uniform id of the object */
	int _id = uuid++;
	/** id generator */
	static inline int uuid = 0;
};
