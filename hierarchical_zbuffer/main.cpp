#include <direct.h>

#include <iostream>
#include <stdexcept>

#include "application.h"

/* program entry point */
int main() {
	/*auto projection = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 1.0f);
	for (int i = -1000; i <= 1000; i += 200) {
		for (int j = -1000; j <= 1000; j += 200) {
			for (int k = -1000; k <= 1000; k += 200) {
				auto v = glm::vec4(1.0f * i, 1.0f * j, 1.0f * k, 1.0f);
				auto u = projection * v;
				Object3D::print(v); std::cout << " => "; Object3D::print(u); std::cout << "\n";
			}
		}
	}

	Object3D::print(projection);*/

	char buf[1024];
	_getcwd(buf, 1024);
	std::cout << "current directory: " << buf << std::endl;

	try {
		Application app;
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}