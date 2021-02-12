#include "SimulationWindow.h"
#include "../SimUtils/Icon.h"


SimulationWindow::SimulationWindow() : RenderEngine::WindowBase("Marbling Simulation")
{
	GLFWimage* icon = LoadGLFWimage("res/icons/Icon.png");
	glfwSetWindowIcon(_window, 1, icon);
}

void SimulationWindow::OnDraw()
{

}

void SimulationWindow::OnCreate()
{
	Maximize();
	SetBackgroundColor(0.5f, 0.2f, 0.0f, 1.0f);
}
