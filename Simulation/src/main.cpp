#include <iostream>

#include "Core/WindowBase.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Grids/Grid.h"
#include "SimulationCore/SimulationApplication.h"


int main(int argc, char* argv[]) {
	RenderEngine::Log::Init();
	INFO("Main Started");
	
	SimulationApplication sim;
	sim.Start();

	//RenderEngine::Application app("Test", new RenderEngine::WindowBase("Test"));
	//app.Start();


	INFO("FINISHED");
}