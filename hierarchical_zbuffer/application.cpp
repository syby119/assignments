//#define SHOW_CALLBACK
//#define SHOW_RENDER_INFO

#include "application.h"

/*
 * @brief default constructor
 */
Application::Application() {
	if (glfwInit() != GLFW_TRUE) {
		std::cerr << "init glfw failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	_window = glfwCreateWindow(
		_windowWidth, _windowHeight, _windowTitle.c_str(), nullptr, nullptr);

	if (_window == nullptr) {
		std::cerr << "create glfw window failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, this);

	glfwSetCursorPosCallback(_window, _cursorMovedCallback);
	//glfwSetMouseButtonCallback(m_window, mouseClickedCallback);
	//glfwSetScrollCallback(m_window, scrollCallback);
	//
	glfwSetKeyCallback(_window, _keyPressedCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "initialize glad failure" << std::endl;
		exit(EXIT_FAILURE);
	}

	_fpsCamera.setWorldPosition(glm::vec3(0.0f, 0.0f, 10.0f));


	_loadModels();
	_initShader();
	
	//_models[0].setLocalEulerAngles(glm::vec3(glm::radians(60.0f), glm::radians(60.0f), 0.0f), Object3D::RotateOrder::XYZ);
	//_models[0].rotate(glm::vec3(glm::radians(60.0f), 0.0f, 0.0f), Object3D::RotateOrder::ZYX);
	//_models[0].rotate(glm::vec3(0.0f, glm::radians(60.0f), 0.0f), Object3D::RotateOrder::ZYX);
	//_models[0].rotate(glm::vec3(0.0f, 0.0f, glm::radians(60.0f)), Object3D::RotateOrder::ZYX);

	_framebuffer = new Framebuffer(_windowWidth, _windowHeight);

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	for (const auto& model : _models) {
		model.getFaces(_vertices, _indices);
	}

	for (size_t i = 0; i < _indices.size(); i += 3) {
		_triangles.push_back({ 
			_vertices[_indices[i]], _vertices[_indices[i + 1]] , _vertices[_indices[i + 2]] });
	}

	_scanlineRenderer = new ScanlineRenderer(*_framebuffer, _windowWidth, _windowHeight, _triangles, _clearColor);

	_lastTimeStamp = std::chrono::high_resolution_clock::now();
}


/*
 * @brief default destructor
 */
Application::~Application() {
	if (_framebuffer != nullptr) {
		delete _framebuffer;
		_framebuffer = nullptr;
	}

	if (_shader != nullptr) {
		delete _shader;
		_shader = nullptr;
	}

	if (_window != nullptr) {
		glfwDestroyWindow(_window);
		_window = 0;
	}
	glfwTerminate();
}


/*
 * @brief main loop
 */
void Application::run() {
	while (!glfwWindowShouldClose(_window)) {
		_updateTime();
		_handleInput();
		_renderFrame();

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}


/*
 * @brief load models from model path
 */
void Application::_loadModels() {
	for (const auto filepath : _modelFilepaths) {
		std::cout << "loading " + filepath + "..." << std::endl;
		_models.push_back(Model(filepath));
		std::cout << "+ vertices: " << _models.back().getVertexCount() << std::endl;
		std::cout << "+ faces:    " << _models.back().getFaceCount() << std::endl;
	}
}


/* @brief init shaders */
void Application::_initShader() {
	// vertex shader code
	const char* vsCode =
		"#version 330 core\n"
		"layout(location = 0) in vec3 aPosition;\n"
		"layout(location = 1) in vec3 aNormal;\n"
		"layout(location = 2) in vec2 aTexCoord;\n"
		"out vec3 normal;\n"
		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main() {\n"
		"	normal = mat3(transpose(inverse(model))) * aNormal;\n"
		"	gl_Position = projection * view * model * vec4(aPosition, 1.0);\n"
		"}\n";

	// fragment shader code
	const char* fsCode =
		"#version 330 core\n"
		"in vec3 normal;\n"
		"uniform vec3 objectColor;\n"
		"uniform vec3 lightColor;\n"
		"uniform vec3 lightDirection;\n"
		"out vec4 color;\n"
		"void main() {\n"
		"	vec3 ambient = 0.1 * lightColor;\n"
		"	vec3 norm = normalize(normal);\n"
		"	vec3 diffuse = max(dot(norm, lightDirection), 0.0) * lightColor;\n"
		"	vec3 result = (ambient + diffuse) * objectColor;\n"
		"	color = vec4(result, 1.0);\n"
		"}\n";

	_shader = new Shader(vsCode, fsCode);
}


/*
 * @brief update time
 */
void Application::_updateTime() {
	auto now = std::chrono::high_resolution_clock::now();
	_deltaTime = 0.001f * std::chrono::duration<double, std::milli>(now - _lastTimeStamp).count();
	_lastTimeStamp = now;
}


/*
 * @brief response mouse move event
 */
void Application::_cursorMovedCallback(GLFWwindow* window, double xPos, double yPos) {
	Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	app->_mouseInput.move.xCurrent = xPos;
	app->_mouseInput.move.yCurrent = yPos;

#ifdef SHOW_CALLBACK
#ifndef NDEBUG
	std::cout << "cursor moved to (" << xPos << ", " << yPos << ")" << std::endl;
#endif
#endif
}


/*
 * @brief response key press event
 */
void Application::_keyPressedCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS) {
		if (key != GLFW_KEY_UNKNOWN) {
			app->_keyboardInput.keyPressed[key] = true;
		}
	} else if (action == GLFW_RELEASE) {
		if (key != GLFW_KEY_UNKNOWN) {
			app->_keyboardInput.keyPressed[key] = false;
		}
	}

#ifdef SHOW_CALLBACK
#ifndef NDEBUG
	auto getKeyInfo = [](int key, int scancode) {
		std::string keyInfo;
		if (glfwGetKeyName(key, scancode)) {
			keyInfo = std::string(glfwGetKeyName(key, scancode)) + "(" + std::to_string(key) + ")";
		} else {
			keyInfo = std::string("unprintable key") + "(" + std::to_string(key) + ")";
		}

		return keyInfo;
	};

	std::cout << "key " << getKeyInfo(key, scancode) << " pressed" << std::endl;
	std::cout << " ------ Pressed Key Table -------" << std::endl;
	for (int i = 0; i < app->_keyboardInput.keyPressed.size(); ++i) {
		if (app->_keyboardInput.keyPressed[i]) {
			std::cout << "\t" << getKeyInfo(i, 0) << std::endl;
		}
	}
#endif
#endif
}


/*
 * @brief handle input and update camera
 */
void Application::_handleInput() {
	_fpsCamera.update(_keyboardInput, _mouseInput, static_cast<float>(_deltaTime));
	
	if (_keyboardInput.keyPressed[GLFW_KEY_0]) {
		_rendererType = RendererType::GpuRenderer;
	} else if (_keyboardInput.keyPressed[GLFW_KEY_1]) {
		_rendererType = RendererType::ScanLineRenderer;
		_scanlineRenderer->setRenderMode(ScanlineRenderer::RenderMode::Global);
	} else if (_keyboardInput.keyPressed[GLFW_KEY_2]) {
		_rendererType = RendererType::ScanLineRenderer;
		_scanlineRenderer->setRenderMode(ScanlineRenderer::RenderMode::ZBuffer);
	} else if (_keyboardInput.keyPressed[GLFW_KEY_3]) {
		_rendererType = RendererType::ScanLineRenderer;
		_scanlineRenderer->setRenderMode(ScanlineRenderer::RenderMode::HierarchicalZBuffer);
	} else if (_keyboardInput.keyPressed[GLFW_KEY_4]) {
		_rendererType = RendererType::ScanLineRenderer;
		_scanlineRenderer->setRenderMode(ScanlineRenderer::RenderMode::OctreeHierarchicalZBuffer);
	}

	_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent;
}

/*
 * @brief render frame with specified render mode
 */
void Application::_renderFrame() {
	auto start = std::chrono::high_resolution_clock::now();
	switch (_rendererType) {
		case RendererType::GpuRenderer:
			_renderWithGpu();
			break;
		case RendererType::ScanLineRenderer:
			_scanlineRenderer->render(*_framebuffer, 
				_fpsCamera, _models, _objectColor, _lightColor, _lightDirection);
			break;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto milliseconds = std::chrono::duration<double, std::milli>(stop - start).count();

	if (_rendererType == RendererType::GpuRenderer) {
		_windowTitle = "gpu renderer";
	} else {
		switch (_scanlineRenderer->getRenderMode()) {
		case ScanlineRenderer::RenderMode::Global:
			_windowTitle = "scanline renderer global with zbuffer";
			break;
		case ScanlineRenderer::RenderMode::ZBuffer:
			_windowTitle = "scanline renderer local with zbuffer";
			break;
		case ScanlineRenderer::RenderMode::HierarchicalZBuffer:
			_windowTitle = "scanline renderer local with hierarchical zbuffer";
			break;
		case ScanlineRenderer::RenderMode::OctreeHierarchicalZBuffer:
			_windowTitle = "scanline renderer local with octree and hierarchical zBuffer";
			break;
		}
	}

	_windowTitle += "  render time: " + std::to_string(milliseconds) + "ms";
	glfwSetWindowTitle(_window, _windowTitle.c_str());


#ifdef SHOW_RENDER_INFO
	std::cout << "+ render time: " << milliseconds << " ms" << std::endl;
#endif
}


void Application::_renderWithGpu() {
	glEnable(GL_DEPTH_TEST);
	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 p = _fpsCamera.getProjectionMatrix();
	glm::mat4 v = _fpsCamera.getViewMatrix();

	_shader->use();
	_shader->setMat4("projection", p);
	_shader->setMat4("view", v);
	_shader->setVec3("objectColor", _objectColor);
	_shader->setVec3("lightColor", _lightColor);
	_shader->setVec3("lightDirection", _lightDirection);
	
	for (const auto& model : _models) {
		// mvp matrices
		glm::mat4 m = model.getModelMatrix();
		_shader->setMat4("model", m);

		model.draw(*_shader);
	}
	glDisable(GL_DEPTH_TEST);
}
