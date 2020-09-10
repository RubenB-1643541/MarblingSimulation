#include <iostream>

#include "Core/WindowBase.h"
#include "Core.h"
#include "Core/Log.h"


int main(int argc, char* argv[]) {
	RenderEngine::Log::Init();
	RenderEngine::Log::GetClientLogger()->info("Test");
	INFO("TEST");
	INFO("Main Started");
	std::cout << "Hello World!" << std::endl;
	RenderEngine::WindowBase window("Test");
	window.Start();
	while (true)
		window.Update();
	std::cout << "Done" << std::endl;
}