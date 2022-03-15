#include <vulkan/vulkan.h>

#include <stdexcept>
#include <iostream>
#include <cstdlib>

#include "FirstTriangleApp.hpp"

int main() {
	FirstTriangleApp app;

	std::cout << "Hello, Triangle!!" << std::endl;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}