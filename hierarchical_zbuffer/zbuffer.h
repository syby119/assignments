#pragma once

#include <limits>

class Zbuffer {
public:
	Zbuffer(int width, int height)
		: _width(width), _height(height){
		_buffer = new float[static_cast<size_t>(width) * height];
	}

	~Zbuffer() {
		if (_buffer) {
			delete _buffer;
			_buffer = nullptr;
		}
	}

	float get(int x, int y) {
		return _buffer[y * _width + x];
	}

	void set(int x, int y, float z) {
		_buffer[y * _width + x] = z;
	}

	bool testAndSet(int x, int y, float z) {
		const int offset = y * _width + x;
		if (z < _buffer[offset]) {
			_buffer[offset] = z;
			return true;
		} else  {
			return false;
		}
	}

	void clear() {
		const int resolution = _width * _height;
		for (int i = 0; i < resolution; ++i) {
			_buffer[i] = 1.0f;
		}
	}
private:
	float* _buffer = nullptr;
	int _width = 0, _height = 0;
};