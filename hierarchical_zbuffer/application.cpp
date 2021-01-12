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

	if (_renderMode == RenderMode::Gpu) {
		glEnable(GL_DEPTH_TEST);
	}

	_fpsCamera.setWorldPosition(glm::vec3(0.0f, 0.0f, 5.0f));


	_loadModels();
	_initShaders();

	std::vector<Vertex> _vertices;
	std::vector<uint32_t> _indices;
	for (const auto& model : _models) {
		model.getFaces(_vertices, _indices);
	}

	for (size_t i = 0; i < _indices.size(); i += 3) {
		_triangles.push_back({ 
			_vertices[_indices[i]], _vertices[_indices[i + 1]] , _vertices[_indices[i + 2]] });
	}

	// new tree
	switch (_renderMode) {
	case RenderMode::ScanLineZBuffer:

		break;
	case RenderMode::HierarchicalZBuffer:
		_quadTree = new QuadTree(_windowWidth, _windowHeight);
		break;
	case RenderMode::OctreeHierarchicalZBuffer:
		
		break;
	}
	// debug
	/*std::ofstream fileOut("debug.txt");
	std::queue<QuadTreeNode*> q;
	q.push(_quadTree->getRoot());
	while (!q.empty()) {
		int count = q.size();
		while (count--) {
			QuadTreeNode* temp = q.front();
			fileOut << temp->box->xl << "\t" << temp->box->xr << "\t" << temp->box->yl << "\t" << temp->box->yr << std::endl;
			for (int i = 0; i < 4; ++i) {
				if (temp->childExists&(1 << i)) {
					uint32_t locCodeChild = temp->locCode << 2 | i;
					q.push(&(_quadTree->nodes[locCodeChild]));
				}
			}
			q.pop();
		}
		fileOut << std::endl;
	}
	fileOut.close();
	std::cout << "fileOutput end" << std::endl;*/

	_lastTimeStamp = std::chrono::high_resolution_clock::now();
}


/*
 * @brief default destructor
 */
Application::~Application() {
	if (_window != nullptr) {
		glfwDestroyWindow(_window);
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
void Application::_initShaders() {
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
		"out vec4 color;\n"
		"void main() {\n"
		"	vec3 objectColor = vec3(1.0, 0.0, 0.0);\n"
		"	vec3 lightColor = vec3(1.0, 1.0, 1.0);\n"
		"	vec3 ambient = 0.1 * lightColor;\n"
		"	vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));\n"
		"	vec3 norm = normalize(normal);\n"
		"	vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;\n"
		"	vec3 result = (ambient + diffuse) * objectColor;\n"
		"	color = vec4(result, 1.0);\n"
		"}\n";

	_shaders.push_back(Shader(vsCode, fsCode));
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
	_fpsCamera.update(_keyboardInput, _mouseInput, _deltaTime);
	
	//for (auto& keyPress : _keyboardInput.keyPressed) {
	//	keyPress = false;
	//}

	_mouseInput.move.xOld = _mouseInput.move.xCurrent;
	_mouseInput.move.yOld = _mouseInput.move.yCurrent;
}

/*
 * @brief render frame with specified render mode
 */
void Application::_renderFrame() {
	auto start = std::chrono::high_resolution_clock::now();
	switch (_renderMode) {
		case RenderMode::Gpu:
			_renderWithGpu();
			break;
		case RenderMode::ScanLineZBuffer:
			_renderWithScanLineZBuffer();
			break;
		case RenderMode::HierarchicalZBuffer:
			_renderWithScanLineZBuffer();
			break;
		case RenderMode::OctreeHierarchicalZBuffer:
			_renderWithScanLineZBuffer();
			break;
	}
	auto stop = std::chrono::high_resolution_clock::now();
	auto milliseconds = std::chrono::duration<double, std::milli>(stop - start).count();

	//std::cout << "+ render time: " << milliseconds << " ms" << std::endl;
}

void Application::_renderWithGpu() {
	glClearColor(_clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 p = _fpsCamera.getProjectionMatrix();
	//std::cout << "projection matrix: " << std::endl;
	//Object3D::print(p);
	//std::cout << std::endl;

	glm::mat4 v = _fpsCamera.getViewMatrix();
	//std::cout << "view matrix: " << std::endl;
	//Object3D::print(v);
	//std::cout << std::endl;

	_shaders[0].use();
	_shaders[0].setMat4("projection", p);
	_shaders[0].setMat4("view", v);
	
	for (const auto& model : _models) {
		// mvp matrices
		glm::mat4 m = model.getModelMatrix();
		//std::cout << "model matrix: " << std::endl;
		//Object3D::print(m);
		//std::cout << std::endl;
		_shaders[0].setMat4("model", m);

		model.draw(_shaders[0]);
	}

	glfwSwapBuffers(_window);
}

void Application::_renderWithScanLineZBuffer() {
	/* write your code here */
}

void Application::_renderWithHierarchicalZBuffer() {
	glm::mat4x4 view = _fpsCamera.getViewMatrix();
	glm::mat4x4 projection = _fpsCamera.getProjectionMatrix();
	for (int i = 0; i < _triangles.size(); ++i) {
		int screenX[3], screenY[3];
		float screenZ[3];
		int maxZ = _quadTree->calTriangle(_triangles[i], view, projection, screenX, screenY, screenZ);
		QuadTreeNode* node = _quadTree->searchNode(screenX, screenY);
		if (node->z < maxZ) {
			// 扫面线遍历三角形，更新quadTree
			float cos = glm::dot(_quadTree->lightDirection, _triangles[i].v->normal);
			if (cos <= 0)
				continue;
			glm::vec3 color = cos * _quadTree->lightColor;
			_quadTree->renderTriangle(screenX, screenY, screenZ, color);
		}
	}

	// 取用frameBuffer 渲染一帧

}

void Application::_renderWithOctreeHierarchicalZBuffer() {
	/* write your code here */
}

