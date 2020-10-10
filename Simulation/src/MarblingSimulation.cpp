#include "MarblingSimulation.h"
#include "Tools/Movement/MouseMovement.h"
#include "Tools/Surface/Square.h"

MarblingSimulation::MarblingSimulation(int sizex, int sizey) : Simulation(sizex, sizey)
{
}

void MarblingSimulation::OnInit()
{
	_settings.fps = 30;
	CreateTools();
}

void MarblingSimulation::OnUpdate()
{

}

void MarblingSimulation::OnDraw()
{
	glUseProgram(NULL);
	glBegin(GL_TRIANGLES);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(10.5f, 10.0f, 1.0f);
	glVertex3f(20.0f, 0.0f, 1.0f);
	glEnd();
}

void MarblingSimulation::CreateTools()
{
	FluidLib::Action<IFrequency>* addfreq = new FluidLib::Action<IFrequency>(IFrequency(16), static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	FluidLib::ToolBase* addfreqtool = new FluidLib::ToolBase();
	addfreqtool->SetAction(addfreq);
	addfreqtool->SetMovement(new FluidLib::MouseMovement());
	addfreqtool->SetSurface(new FluidLib::Square());
	_tools.AddTool(addfreqtool);
	_tools.SetActive(addfreqtool);
}
