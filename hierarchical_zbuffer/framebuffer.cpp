#include "framebuffer.h"
#include <algorithm>


Framebuffer::Framebuffer(int width, int height) :
	_width(width), _height(height) {
	_initQuad();
	_initShader();
	_initTexture();
}


Framebuffer::Framebuffer(Framebuffer&& framebuffer) noexcept {
	_texture = framebuffer._texture;
	framebuffer._texture = 0;

	_pixels = framebuffer._pixels;
	framebuffer._pixels = nullptr;

	_width = framebuffer._width;
	_height = framebuffer._height;

	_vao = framebuffer._vao;
	framebuffer._vao = 0;

	_vbo = framebuffer._vbo;
	framebuffer._vbo = 0;

	_ebo = framebuffer._ebo;
	framebuffer._ebo = 0;

	for (int i = 0; i < 16; ++i) {
		_quadVertices[i] = framebuffer._quadVertices[i];
	}

	for (int i = 0; i < 6; ++i) {
		_quadIndices[i] = framebuffer._quadIndices[i];
	}
}


Framebuffer::~Framebuffer() {
	if (_pixels) {
		delete _pixels;
		_pixels = nullptr;
	}

	if (_texture) {
		glDeleteTextures(1, &_texture);
		_texture = 0;
	}

	if (_shader) {
		delete _shader;
		_shader = nullptr;
	}

	if (_vao) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}

	if (_vbo) {
		glDeleteBuffers(1, &_vbo);
		_vbo = 0;
	}

	if (_ebo) {
		glDeleteBuffers(1, &_ebo);
		_ebo = 0;
	}
}


void Framebuffer::setPixel(int x, int y, const glm::vec3& color) {
	const int offset = 3 * (y * _width + x);
	_pixels[offset] = static_cast<uint8_t>(255 * std::clamp(color.r, 0.0f, 1.0f));
	_pixels[offset + 1] = static_cast<uint8_t>(255 * std::clamp(color.g, 0.0f, 1.0f));
	_pixels[offset + 2] = static_cast<uint8_t>(255 * std::clamp(color.b, 0.0f, 1.0f));
}


void Framebuffer::clear(const glm::vec3& color) {
	const int size = _width * _height * 3;
	for (int i = 0; i < size; i += 3) {
		_pixels[i] = static_cast<uint8_t>(255 * color.r);
		_pixels[i + 1] = static_cast<uint8_t>(255 * color.g);
		_pixels[i + 2] = static_cast<uint8_t>(255 * color.b);
	}
}


void Framebuffer::render() const {
	_shader->use();
	
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _pixels);
	
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Framebuffer::_initQuad() {
	// generate geometry data for a quad representing the image to screen
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_quadVertices), _quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_quadIndices), _quadIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}


void Framebuffer::_initShader() {
	// generate the shader to the quad
	const char* vsCode =
		"#version 330 core\n"
		"layout (location = 0) in vec2 position;\n"
		"layout (location = 1) in vec2 texCoord;\n"
		"out vec2 TexCoord;\n"
		"void main() {\n"
		"gl_Position = vec4(position, 0.0f, 1.0f);\n"
		"TexCoord = texCoord;\n"
		"}\n";

	const char* fsCode =
		"#version 330 core\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main() {\n"
		"color = texture(ourTexture, TexCoord);\n"
		"}\n";

	_shader = new Shader(vsCode, fsCode);
}


void Framebuffer::_initTexture() {
	// allocate memory for pixels
	_pixels = new uint8_t[static_cast<size_t>(_width) * _height * 3];

	// generate texture data for a quad representing the image to screen
	glGenTextures(1, &_texture);

	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}