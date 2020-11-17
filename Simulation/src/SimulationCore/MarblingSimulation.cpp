#include "MarblingSimulation.h"
#include "Tools/Movement/Movements.h"
#include "Tools/Surface/Surfaces.h"
#include "Tools/Action/Actions.h"

MarblingSimulation::MarblingSimulation()
{
}

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
	//src = _grids.GetGrid("Freq")->GetBufferData().id;
	//des = _grids.GetGrid("Freq2")->GetBufferData().id;
	//int size = _grids.GetGrid("Freq2")->GetBufferData().size * sizeof(IFrequency);
	//glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Vel")->GetBufferData().id;
	des = _grids.GetGrid("Vel2")->GetBufferData().id;
	int size = _grids.GetGrid("Vel2")->GetBufferData().size * sizeof(IVelocity);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Ink")->GetBufferData().id;
	des = _grids.GetGrid("Ink2")->GetBufferData().id;
	size = _grids.GetGrid("Ink2")->GetBufferData().size * sizeof(IInk);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);
}

void MarblingSimulation::AfterUpdate()
{
	//Copy freq2 to freq
	GLuint src, des;
	//src = _grids.GetGrid("Freq2")->GetBufferData().id;
	//des = _grids.GetGrid("Freq")->GetBufferData().id;
	//int size = _grids.GetGrid("Freq")->GetBufferData().size * sizeof(IFrequency);
	//glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Vel2")->GetBufferData().id;
	des = _grids.GetGrid("Vel")->GetBufferData().id;
	int size = _grids.GetGrid("Vel")->GetBufferData().size * sizeof(IVelocity);
	glCopyNamedBufferSubData(src, des, NULL, NULL, size);

	src = _grids.GetGrid("Ink2")->GetBufferData().id;
	des = _grids.GetGrid("Ink")->GetBufferData().id;
	size = _grids.GetGrid("Ink")->GetBufferData().size * sizeof(IInk);
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
	//FluidLib::Action<IFrequency>* addfreq = new FluidLib::Action<IFrequency>(IFrequency(16), static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	//FluidLib::Action<IVelocity>* addvel = new FluidLib::Action<IVelocity>(IVelocity(100, 100), static_cast<FluidLib::Grid<IVelocity>*>(_grids.GetGrid("Vel")), FluidLib::ACTION_OPERATION::MOVE);
	
	FluidLib::Action<IFrequency>* addfreq = new FluidLib::Action<IFrequency>(IFrequency(100), static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	FluidLib::Action<IInk>* addink = new FluidLib::Action<IInk>(IInk{ 100,1, {0,0},{1,1,1},0 }, static_cast<FluidLib::Grid<IInk>*>(_grids.GetGrid("Ink")), FluidLib::ACTION_OPERATION::ADD);
	FluidLib::Action<IVelocity>* addvel = new FluidLib::Action<IVelocity>(IVelocity(10, 10), static_cast<FluidLib::Grid<IVelocity>*>(_grids.GetGrid("Vel")), FluidLib::ACTION_OPERATION::MOVE);

	FluidLib::CopyAction<IFrequency>* copyfreq = new FluidLib::CopyAction<IFrequency>(static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")));
	FluidLib::PasteAction<IFrequency>* pastefreq = new FluidLib::PasteAction<IFrequency>(static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), FluidLib::ACTION_OPERATION::ADD);
	FluidLib::CutAction<IFrequency>* cutfreq = new FluidLib::CutAction<IFrequency>(static_cast<FluidLib::Grid<IFrequency>*>(_grids.GetGrid("Freq")), IFrequency(0));

	FluidLib::Action<IInk>* removeink = new FluidLib::Action<IInk>(IInk{ 0,0, {0,0},{0,0,0},0 }, static_cast<FluidLib::Grid<IInk>*>(_grids.GetGrid("Ink")), FluidLib::ACTION_OPERATION::SET);


	basic->AddAction("AddFreq", addfreq);
	basic->AddAction("AddInk", addink);
	basic->AddAction("RemoveInk", removeink);
	basic->AddAction("AddVel", addvel);
	basic->SetActiveAction("AddInk");
	basic->AddMovement("Mouse", new FluidLib::MouseMovement());
	basic->AddMovement("Line", new FluidLib::Line());
	basic->AddMovement("Sine", new FluidLib::Sine());
	basic->AddMovement("BezierCurve", new FluidLib::BezierCurve());
	basic->SetActiveMovement("Mouse");
	basic->AddSurface("Square", new FluidLib::Square());
	basic->AddSurface("Rectangle", new FluidLib::Rectangle());
	basic->AddSurface("Triangle", new FluidLib::Triangle());
	basic->AddSurface("Circle", new FluidLib::Circle());
	basic->AddSurface("Point", new FluidLib::PointSurface());
	basic->SetActiveSurface("Circle");
	_tools.AddTool("Basic", basic);
	
	_tools.SetActive(basic);

	FluidLib::SelectTool * sel = new FluidLib::SelectTool();
	sel->SetAction(addfreq);
	sel->SetCopyAction(copyfreq);
	sel->SetPasteAction(pastefreq);
	sel->SetCutAction(cutfreq);
	FluidLib::Rectangle* rect = static_cast<FluidLib::Rectangle*>(sel->GetSurface());
	copyfreq->SetDim(rect->GetWidthPtr(), rect->GetHeightPtr());
	pastefreq->SetPos(rect->GetXPtr(), rect->GetYPtr());
	cutfreq->SetDim(rect->GetWidthPtr(), rect->GetHeightPtr());
	_tools.AddTool("Select", sel);
	//_tools.SetActive("Select");

}

void MarblingSimulation::InitBasicToolComponent(ToolSelectComponent* comp)
{
	comp->AddButton(Button("res/icons/Mouse.png", "Mouse", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/Line.png", "Line", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/Sine.png", "Sine", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/BezierCurve.png", "BezierCurve", TOOL_PART::MOVEMENT));
	comp->AddButton(Button("res/icons/Ink.png", "AddFreq", TOOL_PART::ACTION));
	comp->AddButton(Button("res/icons/Ink.png", "AddInk", TOOL_PART::ACTION));
	comp->AddButton(Button("res/icons/RemoveInk.png", "RemoveInk", TOOL_PART::ACTION));
	comp->AddButton(Button("res/icons/Force.png", "AddVel", TOOL_PART::ACTION));
	comp->AddButton(Button("res/icons/Square.png", "Square", TOOL_PART::SURFACE));
	comp->AddButton(Button("res/icons/Rectangle.png", "Rectangle", TOOL_PART::SURFACE));
	comp->AddButton(Button("res/icons/Triangle.png", "Triangle", TOOL_PART::SURFACE));
	comp->AddButton(Button("res/icons/Circle.png", "Circle", TOOL_PART::SURFACE));
	comp->AddButton(Button("res/icons/Point.png", "Point", TOOL_PART::SURFACE));
}
