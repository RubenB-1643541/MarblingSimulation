#include "SimulationWindow.h"

SimulationWindow::SimulationWindow() : RenderEngine::WindowBase("Marbling Simulation")
{
	
}

void SimulationWindow::OnDraw()
{

}

void SimulationWindow::OnCreate()
{
	Maximize();
	SetBackgroundColor(0.5f, 0.2f, 0.0f, 1.0f);
}
