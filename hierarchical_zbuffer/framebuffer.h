#pragma once

#include <stdint.h>
#include <glad/glad.h>

#include "shader.h"

class Framebuffer {
public:
	Framebuffer(int width, int height);

	Framebuffer(Framebuffer&& framebuffer) noexcept;

	~Framebuffer();

	void clear(const glm::vec3& color);

	void setPixel(int x, int y, const glm::vec3& color);

	void render() const;
private:
	GLuint _texture = 0;
	uint8_t* _pixels = nullptr;
	int _width = 0, _height = 0;

	GLuint _vao = 0, _vbo = 0, _ebo = 0;

	GLfloat _quadVertices[16] = {
	//   ----xy----   ----uv----
		 1.0f,  1.0f, 1.0f, 1.0f,
	 	 1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	GLuint _quadIndices[6] = {
		0, 1, 3,
		1, 2, 3
	};

	Shader* _shader = nullptr;

	void _initQuad();

	void _initShader();

	void _initTexture();
};