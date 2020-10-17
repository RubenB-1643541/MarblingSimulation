#include "SimulationApplication.h"
#include "GridStructures.h"
#include "SimulationController.h"

SimulationApplication::SimulationApplication() : Application("Marbling Simulation", new SimulationWindow()), _sim(1000,500)
{
	_buffers.clear();
	RenderEngine::ShaderStorageBuffer* velbuf = new RenderEngine::ShaderStorageBuffer();
	velbuf->Bind();
	velbuf->BufferData(_sim.GetSize() * sizeof(IVelocity));
	IVelocity* vels = (IVelocity*)velbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		vels[i] = { 0, 0 };
	}
	velbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Vel", velbuf));

	RenderEngine::ShaderStorageBuffer* velbuf2 = new RenderEngine::ShaderStorageBuffer();
	velbuf2->Bind();
	velbuf2->BufferData(_sim.GetSize() * sizeof(IVelocity));
	IVelocity* vels2 = (IVelocity*)velbuf2->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		vels2[i] = { 0, 0 };
	}
	velbuf2->UnMapBuffer();
	_buffers.insert(std::make_pair("Vel2", velbuf2));

	RenderEngine::ShaderStorageBuffer* freqbuf = new RenderEngine::ShaderStorageBuffer();
	freqbuf->Bind();
	freqbuf->BufferData(_sim.GetSize() * sizeof(IFrequency));
	IFrequency * freqs = (IFrequency*)freqbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		freqs[i] = { 0 };
	}
	freqbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Freq", freqbuf));

	RenderEngine::ShaderStorageBuffer* freqbuf2 = new RenderEngine::ShaderStorageBuffer();
	freqbuf2->Bind();
	freqbuf2->BufferData(_sim.GetSize() * sizeof(IFrequency));
	IFrequency* freqs2 = (IFrequency*)freqbuf2->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		freqs2[i] = { 0 };
	}
	freqbuf2->UnMapBuffer();
	_buffers.insert(std::make_pair("Freq2", freqbuf2));

	RenderEngine::ShaderStorageBuffer* inkbuf = new RenderEngine::ShaderStorageBuffer();
	inkbuf->Bind();
	inkbuf->BufferData(_sim.GetSize() * sizeof(IInk));
	IInk* inks = (IInk*)inkbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		inks[i] = { 0 };
	}
	inkbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Ink", inkbuf));

	RenderEngine::ShaderStorageBuffer* flagbuf = new RenderEngine::ShaderStorageBuffer();
	flagbuf->Bind();
	flagbuf->BufferData(_sim.GetSize() * sizeof(Flags));
	Flags* flags = (Flags*)flagbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		flags[i] = {0, 0, 0, 0 };
	}
	flagbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Flag", flagbuf));

	_computeshader.Create("res/computeshaders/StamAdvection.glsl");
	_shader.Create(_shaderdata);
	
}

SimulationApplication::~SimulationApplication()
{
	for (auto ssb : _buffers) {
		ssb.second->Destroy();
		delete ssb.second;
	}
	_buffers.clear();
}

void SimulationApplication::OnStart()
{
	FluidLib::GridManager * gridman = _sim.GetGrids();
	
	FluidLib::Grid<IVelocity>* velgrid = new FluidLib::Grid<IVelocity>(_buffers.at("Vel")->GetId(), _sim.GetSize(), 1);
	velgrid->SetElementSize(2);
	gridman->AddGrid("Vel", velgrid);
	
	FluidLib::Grid<IVelocity>* velgrid2 = new FluidLib::Grid<IVelocity>(_buffers.at("Vel2")->GetId(), _sim.GetSize(), 2);
	velgrid->SetElementSize(2);
	gridman->AddGrid("Vel2", velgrid2);

	FluidLib::Grid<IFrequency>* freqgrid = new FluidLib::Grid<IFrequency>(_buffers.at("Freq")->GetId(), _sim.GetSize(), 3, true);
	gridman->AddGrid("Freq", freqgrid);

	FluidLib::Grid<IFrequency>* freqgrid2 = new FluidLib::Grid<IFrequency>(_buffers.at("Freq2")->GetId(), _sim.GetSize(), 4);
	gridman->AddGrid("Freq2", freqgrid2);

	FluidLib::Grid<IInk>* inkgrid = new FluidLib::Grid<IInk>(_buffers.at("Ink")->GetId(), _sim.GetSize(), 5, true);
	gridman->AddGrid("Ink", inkgrid);

	FluidLib::Grid<Flags>* flaggrid = new FluidLib::Grid<Flags>(_buffers.at("Flag")->GetId(), _sim.GetSize(), 7, true);
	flaggrid->SetElementSize(4);
	gridman->AddGrid("Flag", flaggrid);

	FluidLib::ComputeShaderController * shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));

	FluidLib::GridRenderer* renderer = _sim.GetRenderer();
	renderer->SetShader(_shader.GetId());
	FluidLib::ShaderController* shader = renderer->GetShader();
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));

	_sim.Init();
	CreateInterface();
}

void SimulationApplication::OnStop()
{
	_sim.Stop();
}

void SimulationApplication::OnUpdate()
{
	//RenderEngine::ShaderStorageBuffer* velbuf = _buffers.at("Vel2");
	//velbuf->Bind();
	//IVelocity* freqs = (IVelocity*)velbuf->MapBufferRange();
	//INFO(freqs[0].dx);
	//velbuf->UnMapBuffer();


	_sim.Update();
	_sim.Draw();
}

void SimulationApplication::CreateInterface()
{
	_interface.AddComponent("Left", new SideComponent("Left", ImVec2(0, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	_interface.AddComponent("Right", new SideComponent("Right", ImVec2(RenderEngine::Application::Get()->GetWidth() - RenderEngine::Application::Get()->GetWidth()/5, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	_interface.AddComponent("Menu", new TopBar());
	_interface.GetComponent("Right")->AddComponent(new ToolParameters(_sim.GetTools()));
	ToolSelectComponent* toolselect = new ToolSelectComponent(_sim.GetTools(), "Basic");
	_interface.GetComponent("Left")->AddComponent(toolselect);
	_sim.InitBasicToolComponent(toolselect);
}

bool SimulationApplication::OnWindowResizeEvent(RenderEngine::WindowResizeEvent& e)
{
	_sim.SetScreenSize((float)e.GetWidth(), (float)e.GetHeight());
	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	INFO("Window Resize {} - {}", e.GetWidth(), e.GetHeight());
	return true;
}

bool SimulationApplication::OnMouseMoveEvent(RenderEngine::MouseMoveEvent& e)
{
	FluidLib::SimulationController::MouseMove(e.GetX(), e.GetY());
	return false;
}

bool SimulationApplication::OnMousePressEvent(RenderEngine::MousePressEvent& e)
{
	FluidLib::SimulationController::MouseClick(e.GetButton());
	return false;
}

bool SimulationApplication::OnMouseReleaseEvent(RenderEngine::MouseReleaseEvent& e)
{
	FluidLib::SimulationController::MouseRelease(e.GetButton());
	return false;
}

bool SimulationApplication::OnMouseScrollEvent(RenderEngine::MouseScrollEvent& e)
{
	FluidLib::SimulationController::MouseScroll(e.GetX(), e.GetY());
	return false;
}