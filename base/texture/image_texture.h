#pragma once

#include <cmath>
#include <string>
#include <stdexcept>
#include <stb_image.h>

#include "./texture.h"

class ImageTexture : public Texture {
public:
	ImageTexture(const std::string& filepath) {
		// follow the opengl convention
		stbi_set_flip_vertically_on_load(true);
		_data = stbi_load(filepath.c_str(), &_width, &_height, &_channels, 3);
		_channels = 3;
		
		if (_data == nullptr) {
			throw std::runtime_error("Load " + filepath + " failure");
		}
	}

	~ImageTexture() {
		if (_data != nullptr) {
			stbi_image_free(_data);
			_data = nullptr;
		}
	}

	gtm::Vector3<float> sampleColor(const gtm::Vector2<float>& texCoord) const override {
		// to maintain the return value of 3d vector
		// much more works to do
		assert(_channels == 3);
		// repeat texture
		float u = texCoord.u - std::floor(texCoord.u);
		float v = texCoord.v - std::floor(texCoord.v);
		// get the pixel the texture coord belongs to
		int offset = (static_cast<int>(_width * u) + _width * static_cast<int>(_height * v)) * _channels;
		
		constexpr float s = 1.0f / 255.0f;
		return { s * _data[offset], s * _data[offset + 1], s * _data[offset + 2] };
	}
private:
	unsigned char* _data = nullptr;
	int _width = 0, _height = 0, _channels = 0;
};