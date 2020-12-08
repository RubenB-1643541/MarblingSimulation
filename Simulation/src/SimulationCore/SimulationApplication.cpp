#include "SimulationApplication.h"
#include "GridStructures.h"
#include "SimulationController.h"


SimulationApplication::SimulationApplication() : Application("Marbling Simulation", new SimulationWindow())
{
	
	_computeshader.Create("res/computeshaders/StamAdvection_Colors.glsl");
	_shader.Create(_shaderdata);

	InitShortCuts();
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
	CreateInterface();
	
}

void SimulationApplication::OnStop()
{
	_sim.Stop();
}

void SimulationApplication::OnUpdate()
{
	

	if (_simrunning) {
		//RenderEngine::ShaderStorageBuffer* freq = _buffers.at("Freq");
		//freq->Bind();
		//IFrequency* freqs = (IFrequency*)freq->MapBufferRange();
		//INFO(freqs[0].freq);
		//freq->UnMapBuffer();

		_sim.Update();
		_sim.Draw();
	}
}

void SimulationApplication::CreateInterface()
{
	_interface.AddComponent("Left", new SideComponent("Left", ImVec2(0, 30), ImVec2(1920 / 5, 1001 - 50)));
	//_interface.AddComponent("Left", new SideComponent("Left", ImVec2(0, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	_interface.AddComponent("Right", new SideComponent("Right", ImVec2(1920 - 1920/5, 30), ImVec2(1920 / 5, 1001 - 50)));
	//_interface.AddComponent("Right", new SideComponent("Right", ImVec2(RenderEngine::Application::Get()->GetWidth() - RenderEngine::Application::Get()->GetWidth()/5, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	_interface.AddComponent("Menu", new TopBar());
	_interface.GetComponent("Right")->AddComponent(new ToolParameters(_sim.GetTools()));
	ToolSelectComponent* toolselect = new ToolSelectComponent(_sim.GetTools(), "Basic");
	PresetComponent* preset = new PresetComponent(_sim.GetTools(), "Basic");
	_interface.GetComponent("Left")->AddComponent(new SettingsComponent());
	_interface.GetComponent("Left")->AddComponent(toolselect);
	_interface.GetComponent("Left")->AddComponent(preset);
	_sim.InitBasicToolComponent(toolselect);
	_interface.AddComponent("Create", new CreateComponent(this, true));
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

inline bool SimulationApplication::OnKeyPressEvent(RenderEngine::KeyPressEvent& e)
{
	FluidLib::SimulationController::KeyPress(e.GetKey());
	_shortcuts.OnKeyPressEvent(e);
	return false;
}

inline bool SimulationApplication::OnKeyReleaseEvent(RenderEngine::KeyReleaseEvent& e)
{
	FluidLib::SimulationController::KeyRelease(e.GetKey());
	_shortcuts.OnKeyReleaseEvent(e);
	return false;
}

void SimulationApplication::StartSimulation()
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
	IFrequency* freqs = (IFrequency*)freqbuf->MapBufferRange();
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
		inks[i] = { 0, 0, {0,0}, {0,0,0},0 };
	}
	inkbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Ink", inkbuf));

	RenderEngine::ShaderStorageBuffer* inkbuf2 = new RenderEngine::ShaderStorageBuffer();
	inkbuf2->Bind();
	inkbuf2->BufferData(_sim.GetSize() * sizeof(IInk));
	IInk* inks2 = (IInk*)inkbuf2->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		inks2[i] = { 0, 0, {0,0}, {0,0,0},0 };
	}
	inkbuf2->UnMapBuffer();
	_buffers.insert(std::make_pair("Ink2", inkbuf2));

	RenderEngine::ShaderStorageBuffer* flagbuf = new RenderEngine::ShaderStorageBuffer();
	flagbuf->Bind();
	flagbuf->BufferData(_sim.GetSize() * sizeof(Flags));
	Flags* flags = (Flags*)flagbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		flags[i] = { 0, 0, 0, 0 };
	}
	flagbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Flag", flagbuf));

	FluidLib::GridManager* gridman = _sim.GetGrids();

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

	FluidLib::Grid<IInk>* inkgrid2 = new FluidLib::Grid<IInk>(_buffers.at("Ink2")->GetId(), _sim.GetSize(), 6, true);
	gridman->AddGrid("Ink2", inkgrid2);

	FluidLib::Grid<Flags>* flaggrid = new FluidLib::Grid<Flags>(_buffers.at("Flag")->GetId(), _sim.GetSize(), 7, true);
	flaggrid->SetElementSize(4);
	gridman->AddGrid("Flag", flaggrid);

	FluidLib::ComputeShaderController* shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	FluidLib::UniformVal aval; aval.floatptr = &_sim.GetSettings()->spreading;
	FluidLib::UniformVal dval; dval.floatptr = &_sim.GetSettings()->diffuse;
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, aval, "a"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, dval, "d"));

	FluidLib::GridRenderer* renderer = _sim.GetRenderer();
	renderer->SetShader(_shader.GetId());
	FluidLib::ShaderController* shader = renderer->GetShader();
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));


	_sim.Init();
	_simrunning = true;
}

void SimulationApplication::LoadSimulation(const std::string& file)
{
	SimLoad loader;
	loader.SetFile(file);
	loader.SetBuffers(&_buffers);
	loader.SetGrids(_sim.GetGrids());
	loader.Load();

	FluidLib::ComputeShaderController* shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	FluidLib::UniformVal aval; aval.floatptr = &_sim.GetSettings()->spreading;
	FluidLib::UniformVal dval; dval.floatptr = &_sim.GetSettings()->diffuse;
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, aval, "a"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, dval, "d"));
	
	FluidLib::GridRenderer* renderer = _sim.GetRenderer();
	renderer->SetShader(_shader.GetId());
	FluidLib::ShaderController* shader = renderer->GetShader();
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	
	
	_sim.Init();
	_simrunning = true;
}

void SimulationApplication::InitShortCuts()
{
	_shortcuts.AddShortCut({ KEY_D , false, true,[]() {//Shift D select select tool
		INFO("SELECT TOOL");
		FluidLib::Simulation::Get()->GetTools()->SetActive("Select");
	} });

	_shortcuts.AddShortCut({ KEY_B , false, true,[]() {//Shift B select basic tool
		INFO("BASIC TOOL");
		FluidLib::Simulation::Get()->GetTools()->SetActive("Basic");
	} });

	_shortcuts.AddShortCut({ KEY_P , false, false,[]() {//P toggle pause
		INFO("TOGGLE PAUSE");
		static int last = 0;
		static bool pause = false;
		if (!pause) {
			last = FluidLib::Simulation::Get()->GetSettings()->fps;
			FluidLib::Simulation::Get()->GetSettings()->fps = 0;
			pause = true;
		}
		else {
			FluidLib::Simulation::Get()->GetSettings()->fps = last;
			pause = false;
		}
	} });

	_shortcuts.AddShortCut({ KEY_C , true, false,[]() {//Ctrl C copy with select tool
		INFO("COPY");
		FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
		if (tool->GetName() == "Select") {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
			select->Copy();
		}
	} });
	_shortcuts.AddShortCut({ KEY_V , true, false,[]() {//Ctrl V paste with select tool
		INFO("PASTE");
		FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
		if (tool->GetName() == "Select") {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
			select->Paste();
		}
	} });

	_shortcuts.AddShortCut({ KEY_X , true, false,[]() {//Ctrl X cut with select tool
		INFO("CUT");
		FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
		if (tool->GetName() == "Select") {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
			select->Cut();
		}
	} });

	_shortcuts.AddShortCut({ KEY_S, true, false, []() {//Ctrl S save
		INFO("SAVE");
		SimSave saver;
		saver.SetFile("Test.sim");
		saver.SetGrids(FluidLib::Simulation::Get()->GetGrids());
		saver.Save();
	} });
}

/*
void SimulationApplication::StartSimulation()
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
	IFrequency* freqs = (IFrequency*)freqbuf->MapBufferRange();
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
		flags[i] = { 0, 0, 0, 0 };
	}
	flagbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Flag", flagbuf));

	FluidLib::GridManager* gridman = _sim.GetGrids();

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

	FluidLib::ComputeShaderController* shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	FluidLib::UniformVal aval; aval.floatptr = &_sim.GetSettings()->spreading;
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, aval, "a"));

	FluidLib::GridRenderer* renderer = _sim.GetRenderer();
	renderer->SetShader(_shader.GetId());
	FluidLib::ShaderController* shader = renderer->GetShader();
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));


	_sim.Init();
	_simrunning = true;
}
*/
