#include <iostream>
#include <string>

#include "Core/WindowBase.h"
#include "Core/Application.h"
#include "Core/Log.h"
#include "Grids/Grid.h"
#include "SimulationCore/SimulationApplication.h"


int main(int argc, char* argv[]) {
	RenderEngine::Log::Init();
	INFO("Main Started");

	SimulationApplication sim;
	if (argc == 2) {
		//Load file
		std::string file = argv[1];
		sim.Start();
		sim.LoadSimulation(file);
	}
	else {
		sim.Start();
	}
	

	//RenderEngine::Application app("Test", new RenderEngine::WindowBase("Test"));
	//app.Start();


	INFO("FINISHED");
}