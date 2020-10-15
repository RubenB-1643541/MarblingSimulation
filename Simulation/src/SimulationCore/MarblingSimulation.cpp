#include "MarblingSimulation.h"
#include "Tools/Movement/MouseMovement.h"
#include "Tools/Movement/Line.h"
#include "Tools/Surface/Square.h"

MarblingSimulation::MarblingSimulation(int sizex, int sizey) : Simulation(sizex, sizey)
{
}

void MarblingSimulation::OnInit()
{
	_settings.fps = 30;
	CreateTools();
}

void MarblingSimulation::BeforeUpdate()
{
	//Copy freq to freq2
	GLuint src, des;
	src = _grids.GetGrid("Freq")->GetBufferData().id;
	des = _grids.GetGrid("Freq2")->GetBufferData().id;
	int size = _grids.GetGrid("Freq2")->GetBufferData().size * sizeof(IFrequency);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Vel")->GetBufferData().id;
	des = _grids.GetGrid("Vel2")->GetBufferData().id;
	size = _grids.GetGrid("Vel2")->GetBufferData().size * sizeof(IVelocity);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);
}

void MarblingSimulation::AfterUpdate()
{
	//Copy freq2 to freq
	GLuint src, des;
	src = _grids.GetGrid("Freq2")->GetBufferData().id;
	des = _grids.GetGrid("Freq")->GetBufferData().id;
	int size = _grids.GetGrid("Freq")->GetBufferData().size * sizeof(IFrequency);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Vel2")->GetBufferData().id;
	des = _grids.GetGrid("Vel")->GetBufferData().id;
	size = _grids.GetGrid("Vel")->GetBufferData().size * sizeof(IVelocity);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);
	
}

void MarblingSimulation::OnDraw()
{
	

}

void MarblingSimulation::CreateTools()
{
	FluidLib::Action<IFrequency>* addfreq = new FluidLib::Action<IFrequency>(IFrequency(16), static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	FluidLib::ToolBase* addfreqtool = new FluidLib::ToolBase();
	addfreqtool->SetAction(addfreq);
	addfreqtool->SetMovement(new FluidLib::Line());
	addfreqtool->SetSurface(new FluidLib::Square());
	_tools.AddTool(addfreqtool);
	_tools.SetActive(addfreqtool);
}
