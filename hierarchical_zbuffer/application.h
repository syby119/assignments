#pragma once

#include <array>
#include <chrono>
#include <cstdlib>
#include <list>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fps_camera.h"
#include "input.h"
#include "model.h"
#include "quadtree.h"
#include "octree.h"
#include "framebuffer.h"
#include "zbuffer.h"
#include "quadtree.h"
#include "clipper.h"
#include "scanline_renderer.h"


class Application {
public:
	/*
	 * @brief default constructor
	 */
	Application();

	/*
	 * @brief default destructor
	 */
	virtual ~Application();

	/*
	 * @brief main loop
	 */
	void run();

	/*
	 * @brief renderer
	 */
	enum class RendererType {
		GpuRenderer, ScanLineRenderer
	};

private:
	/* window */
	GLFWwindow* _window = nullptr;
	std::string _windowTitle = "Hierarchical Z-Buffer";
	int _windowWidth = 1280;
	int _windowHeight = 720;
	glm::vec4 _clearColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	/* time */
	std::chrono::time_point<std::chrono::high_resolution_clock> _lastTimeStamp;
	double _deltaTime = 0.0f;

	/* model */
	std::vector<Model> _models;
	std::vector<std::string> _modelFilepaths{ "../resources/bunny.obj", "../resources/ball.obj" };

	/* triangle data: local space */
	std::vector<Triangle> _triangles;

	/* camera */
	FpsCamera _fpsCamera{glm::radians(54.0f), 1.0f * _windowWidth / _windowHeight, 1.0f, 500.0f };

	/* input */
	KeyboardInput _keyboardInput;
	MouseInput _mouseInput;

	/* shader program for test */
	Shader* _shader = nullptr;

	/* framebuffer */
	Framebuffer* _framebuffer = nullptr;

	/* object color */
	glm::vec3 _objectColor = glm::vec3(0.9f, 0.9f, 0.9f);

	/* light color */
	glm::vec3 _lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	/* light direction */
	glm::vec3 _lightDirection = -glm::normalize(glm::vec3(0.8f, -3.0f, -1.5f));

	/* renderer mode */
	enum RendererType _rendererType = RendererType::GpuRenderer;

	/* renderer */
	ScanlineRenderer* _scanlineRenderer = nullptr;


	/*
	 * @brief load models from model path
	 */
	void _loadModels();

	/* @brief init shader */
	void _initShader();

	/*
	 * @brief update time
	 */
	void _updateTime();

	/*
	 * @brief response mouse move event
	 */
	static void _cursorMovedCallback(GLFWwindow* window, double xPos, double yPos);

	/*
	 * @brief response key press event
	 */
	static void _keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	/*
	 * @brief handle input and update camera
	 */
	void _handleInput();

	/*
	 * @brief render frame with specified render mode
	 */
	void _renderFrame();


	/*
	 * @brief render frame with gpu
	 */
	void _renderWithGpu();
};