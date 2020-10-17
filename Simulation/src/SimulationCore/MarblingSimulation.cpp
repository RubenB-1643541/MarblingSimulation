#include "MarblingSimulation.h"
#include "Tools/Movement/MouseMovement.h"
#include "Tools/Movement/Line.h"
#include "Tools/Surface/Square.h"
#include "Tools/Surface/Circle.h"

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
	CreateBasicTool();
}

void MarblingSimulation::CreateBasicTool()
{
	FluidLib::BasicTool* basic = new FluidLib::BasicTool();
	FluidLib::Action<IFrequency>* addfreq = new FluidLib::Action<IFrequency>(IFrequency(16), static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	//FluidLib::ToolBase* addfreqtool = new FluidLib::ToolBase();
	basic->AddAction("AddFreq", addfreq);
	basic->SetActiveAction("AddFreq");
	basic->AddMovement("Mouse", new FluidLib::MouseMovement());
	basic->AddMovement("Line", new FluidLib::Line());
	basic->SetActiveMovement("Line");
	basic->AddSurface("Square", new FluidLib::Square());
	basic->AddSurface("Circle", new FluidLib::Circle());
	basic->SetActiveSurface("Circle");
	_tools.AddTool("Basic", basic);
	
	_tools.SetActive(basic);
	basic->SetActiveSurface("Square");

}

void MarblingSimulation::InitBasicToolComponent(ToolSelectComponent* comp)
{
	comp->AddButton(Button("res/icons/Mouse.png", "Mouse", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/Line.png", "Line", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/Ink.png", "AddFreq", TOOL_PART::ACTION));
	comp->AddButton(Button("res/icons/Square.png", "Square", TOOL_PART::SURFACE));
	comp->AddButton(Button("res/icons/Circle.png", "Circle", TOOL_PART::SURFACE));
}
