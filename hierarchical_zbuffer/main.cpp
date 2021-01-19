#include <iostream>
#include <stdexcept>

#include "application.h"

/* program entry point */
int main() {
	try {
		Application app;
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}