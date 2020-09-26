#include "SimulationWindow.h"

SimulationWindow::SimulationWindow() : RenderEngine::WindowBase("Marbling Simulation")
{

}

void SimulationWindow::OnDraw()
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-1, -1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(1, -1, 0);

	glEnd();
}
