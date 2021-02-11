#pragma once

#include "input.h"
#include "perspective_camera.h"

class FpsCamera: public PerspectiveCamera {
public:
	FpsCamera(float fov, float aspect, float znear = 0.1f, float zfar = 10000.0f)
		: PerspectiveCamera(fov, aspect, znear, zfar) {
		_name = "fps camera";
	}


	void update(const KeyboardInput& keyboardInput, const MouseInput& mouseInput, float deltaTime) {
		glm::vec3 movement(0.0f);

		if (keyboardInput.keyPressed[GLFW_KEY_W]) {
			movement += _speed * deltaTime * glm::vec3(0.0f, 0.0f, -1.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_A]) {
			movement += _speed * deltaTime * glm::vec3(-1.0f, 0.0f, 0.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_S]) {
			movement += _speed * deltaTime * glm::vec3(0.0f, 0.0f, 1.0f);
		}

		if (keyboardInput.keyPressed[GLFW_KEY_D]) {
			movement += _speed * deltaTime * glm::vec3(1.0f, 0.0f, 0.0f);
		}


		translate(movement, Object3D::Space::Local);
		//printPosition();
		//print(getViewMatrix());
		//printRotation();
		//printScale();

		double xoffset = mouseInput.move.xCurrent - mouseInput.move.xOld;
		// OpenGL flip y-axis 
		double yoffset = -(mouseInput.move.yCurrent - mouseInput.move.yOld);

		glm::vec3 eulerAngles = getLocalEulerAngles(Object3D::RotateOrder::ZYX);
		if (yoffset != 0.0f && xoffset != 0.0f) {
			std::cout << "eulerAngles: "; print(eulerAngles); std::cout << std::endl;
			std::cout << "\t dx = " << xoffset * _sensitivity * deltaTime << ", dy = " << yoffset * _sensitivity * deltaTime << "\n";
			eulerAngles.x += yoffset * _sensitivity * deltaTime;
			eulerAngles.y += xoffset * _sensitivity * deltaTime;
			rotate(eulerAngles, Object3D::RotateOrder::ZYX);
			glm::vec3 eulerAngles = getLocalEulerAngles(Object3D::RotateOrder::ZYX);
			std::cout << "eulerAngles: "; print(eulerAngles); std::cout << std::endl;
			std::cout << "----------------------------------------" << std::endl;
		}
	}

	void lookAt(glm::vec3 worldPosition, glm::vec3 worldUp) override {
		assert("TODO: need to perform look at");
	}

private:
	float _speed = 2.5f;
	float _sensitivity = 0.00001f;
};