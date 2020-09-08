#include <iostream>

#include "Window/WindowBase.h"


int main(int argc, char* argv[]) {
	std::cout << "Hello World!" << std::endl;
	RenderEngine::WindowBase window("Test");
	window.Start();
}