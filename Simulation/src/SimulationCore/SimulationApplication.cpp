#include "SimulationApplication.h"
#include "GridStructures.h"
#include "SimulationController.h"
#include "../FileIO/Importer.h"


SimulationApplication::SimulationApplication() : Application("Marbling Simulation", new SimulationWindow())
{
	
	_computeshader.Create(_computeShader.c_str());
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
	SaveStateHandler::Init();
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
		//INFO("Freq: {}", freqs[0].freq);
		//INFO("Pos : {}", freqs[1].freq);
		//freq->UnMapBuffer();

		_sim.Update();
		_sim.Draw();
	}
}

void SimulationApplication::CreateInterface()
{
	int h = GetHeight();
	int w = GetWidth();
	_interface.AddComponent("Left", new SideComponent("Left", ImVec2(0, 30), ImVec2(w / 5, h - 50)));
	//_interface.AddComponent("Left", new SideComponent("Left", ImVec2(0, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	_interface.AddComponent("Right", new SideComponent("Right", ImVec2(w - w/5, 30), ImVec2(w / 5, h - 50)));
	//_interface.AddComponent("Right", new SideComponent("Right", ImVec2(RenderEngine::Application::Get()->GetWidth() - RenderEngine::Application::Get()->GetWidth()/5, 30), ImVec2(RenderEngine::Application::Get()->GetWidth() / 5, RenderEngine::Application::Get()->GetHeight() - 50)));
	TopBar* top = new TopBar();
	_interface.AddComponent("Menu", top);
	_interface.GetComponent("Right")->AddComponent(new ToolParameters(_sim.GetTools()));
	ToolSelectComponent* toolselect = new ToolSelectComponent(_sim.GetTools(), "Basic");
	PresetComponent* preset = new PresetComponent(_sim.GetTools(), "Basic", toolselect);
	_interface.GetComponent("Left")->AddComponent(new SettingsComponent());
	_interface.GetComponent("Left")->AddComponent(toolselect);
	_interface.GetComponent("Left")->AddComponent(preset);
	_interface.GetComponent("Right")->AddComponent(new MultiSurfaceComponent(_sim.GetTools(), "Basic"));
	_interface.GetComponent("Right")->AddComponent(new SaveStateComponent());
	_sim.InitBasicToolComponent(toolselect);
	_interface.AddComponent("Create", new CreateComponent(this, !_load));
	ExportComponent* exp = new ExportComponent();
	_interface.AddComponent("Export", exp);
	_interface.AddComponent("ToolPopup", new ToolPopupComponent(_sim.GetTools()));
	top->SetExportComponent(exp);
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
	CreateGrids();
	CreateUniforms();

	_sim.Init();
	_simrunning = true;
}

bool SimulationApplication::LoadSimulation(const std::string& file)
{
	SimLoad loader;
	loader.SetFile(file);
	loader.SetBuffers(&_buffers);
	loader.SetGrids(_sim.GetGrids());
	////loader.Load();
	if (!loader.StartLoad()) {
		ERROR("Failed to open file : ");
		ERROR(file.c_str());
		return false;
	}
	if (!loader.LoadSimData()) {
		ERROR("Failed To Load Simulation Data");
		return false;
	}
	CreateGrids();
	loader.LoadGrids();
	loader.EndLoad();

	CreateUniforms();
	
	_sim.Init();
	_simrunning = true;
	int tempfps = _sim.GetSettings()->fps;//Force 1 update for rendering -> if fps = 0 no rendering (for some fucking reason)
	_sim.GetSettings()->fps = 10000;	  //Force 1 update for rendering -> if fps = 0 no rendering (for some fucking reason)
	_sim.Update();						  //Force 1 update for rendering -> if fps = 0 no rendering (for some fucking reason)
	_sim.GetSettings()->fps = tempfps;	  //Force 1 update for rendering -> if fps = 0 no rendering (for some fucking reason)
	if(_load)
		Start();
	return true;
}

bool SimulationApplication::ImportImage(const std::string& file)
{
	Importer importer;
	importer.SetFile(file);
	if (!importer.Import(false))
		return false;
	Image im = importer.GetImportedImage();
	_sim.SetSize(im.width, im.height);
	CreateGrids();
	FluidLib::ColorGrid<IInk>* colorgrid = static_cast<FluidLib::ColorGrid<IInk>*>(_sim.GetGrids()->GetGrid("Ink"));
	//Load Image Data
	importer.ImportInGrid(colorgrid);
	
	CreateUniforms();
	_sim.Init();
	_simrunning = true;
	return true;
}

void SimulationApplication::CreateGrids()
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
		inks[i] = { 0, 0 };
		//inks[i] = { 0, 0, {0,0}, {0,0,0},0 };
	}
	inkbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Ink", inkbuf));

	RenderEngine::ShaderStorageBuffer* inkbuf2 = new RenderEngine::ShaderStorageBuffer();
	inkbuf2->Bind();
	inkbuf2->BufferData(_sim.GetSize() * sizeof(IInk));
	IInk* inks2 = (IInk*)inkbuf2->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		inks2[i] = { 0, 0 };
		//inks2[i] = { 0, 0, {0,0}, {0,0,0},0 };
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

	FluidLib::Grid<IVelocity>* velgrid = new FluidLib::Grid<IVelocity>(_buffers.at("Vel")->GetId(), _sim.GetSize(), 1, true);
	velgrid->SetElementSize(2);
	gridman->AddGrid("Vel", velgrid);

	FluidLib::Grid<IVelocity>* velgrid2 = new FluidLib::Grid<IVelocity>(_buffers.at("Vel2")->GetId(), _sim.GetSize(), 2);
	velgrid->SetElementSize(2);
	gridman->AddGrid("Vel2", velgrid2);

	FluidLib::Grid<IFrequency>* freqgrid = new FluidLib::Grid<IFrequency>(_buffers.at("Freq")->GetId(), _sim.GetSize(), 3);
	gridman->AddGrid("Freq", freqgrid);

	FluidLib::Grid<IFrequency>* freqgrid2 = new FluidLib::Grid<IFrequency>(_buffers.at("Freq2")->GetId(), _sim.GetSize(), 4);
	gridman->AddGrid("Freq2", freqgrid2);

	FluidLib::Grid<IInk>* inkgrid = new FluidLib::ColorGrid<IInk>(_buffers.at("Ink")->GetId(), _sim.GetSize(), 5, 8, true);
	inkgrid->SetElementSize(2);
	gridman->AddGrid("Ink", inkgrid);

	FluidLib::Grid<IInk>* inkgrid2 = new FluidLib::Grid<IInk>(_buffers.at("Ink2")->GetId(), _sim.GetSize(), 6, true);
	inkgrid2->SetElementSize(2);
	gridman->AddGrid("Ink2", inkgrid2);

	FluidLib::Grid<Flags>* flaggrid = new FluidLib::FlagGrid<Flags>(_buffers.at("Flag")->GetId(), _sim.GetSize(), 7, true);
	static_cast<FluidLib::FlagGrid<Flags>*>(flaggrid)->CopyFromGPU();
	flaggrid->SetElementSize(4);
	gridman->AddGrid("Flag", flaggrid);
}

void SimulationApplication::CreateUniforms()
{
	FluidLib::ComputeShaderController* shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	FluidLib::UniformVal aval; aval.floatptr = &_sim.GetSettings()->spreading;
	FluidLib::UniformVal dval; dval.floatptr = &_sim.GetSettings()->diffuse;
	FluidLib::UniformVal intensity; intensity.floatptr = &_sim.GetSettings()->intesity;
	FluidLib::UniformVal freezeintensity; freezeintensity.floatptr = &_sim.GetSettings()->freezeintensity;
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, aval, "a"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, dval, "d"));
	//shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT_PTR, intensity, "intensity"));

	FluidLib::UniformVal renderforce; renderforce.floatptr = (float*) &_sim.GetSettings()->renderforce;

	FluidLib::GridRenderer* renderer = _sim.GetRenderer();
	renderer->SetShader(_shader.GetId());
	FluidLib::ShaderController* shader = renderer->GetShader();
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "width"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "height"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, intensity, "intensity"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, freezeintensity, "freezeintensity"));
	shader->AddUniform(FluidLib::Uniform(FluidLib::UniformType::FLOAT_PTR, renderforce, "renderforce"));
}

void SimulationApplication::InitShortCuts()
{
	_shortcuts.AddShortCut({ KEY_S , false, true,[]() {//Shift S select select tool
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
			FluidLib::ColorGrid<IInk> * col = static_cast<FluidLib::ColorGrid<IInk>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
			Image im = ImageFromClipboard(col->GetColors(), FluidLib::Simulation::Get()->GetSettings()->intesity);
			FluidLib::TextureData texdat = { im.width, im.height, 4, im.data };
			select->Paste(texdat);
		}
	} });

	_shortcuts.AddShortCut({ KEY_V , true, true,[]() {//Ctrl Shift V hardpaste with select tool
	INFO("HARDPASTE");
	FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
	if (tool->GetName() == "Select") {
		FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
		FluidLib::ColorGrid<IInk>* col = static_cast<FluidLib::ColorGrid<IInk>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
		Image im = ImageFromClipboard(col->GetColors(), FluidLib::Simulation::Get()->GetSettings()->intesity);
		FluidLib::TextureData texdat = { im.width, im.height, 4, im.data };
		select->HardPaste(texdat);
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

	_shortcuts.AddShortCut({ KEY_F , true, false,[]() {//Ctrl F freeze with select tool
		INFO("FREEZE");
		FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
		if (tool->GetName() == "Select") {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
			select->ExecuteAction("Freeze");
		}
	} });

	_shortcuts.AddShortCut({ KEY_F , true, true,[]() {//ctrl shift F unfreeze with select tool
		INFO("UNFREEZE");
		FluidLib::ToolBase* tool = FluidLib::Simulation::Get()->GetTools()->GetActive();
		if (tool->GetName() == "Select") {
			FluidLib::SelectTool* select = static_cast<FluidLib::SelectTool*>(tool);
			select->ExecuteAction("Unfreeze");
		}
	} });

	_shortcuts.AddShortCut({ KEY_S, true, false, []() {//Ctrl S save
		INFO("SAVE");
		SimSave saver;
		//saver.SetFile("Test.sim");
		saver.SetGrids(FluidLib::Simulation::Get()->GetGrids());
		saver.Save();
	} });

	_shortcuts.AddShortCut({ KEY_E, true, false,[]() {//Ctrl E create save state
		INFO("CREATE SAVE STATE");
		SaveStateHandler::CreateSaveState();
	} });

	_shortcuts.AddShortCut({ KEY_W, true, false,[]()  {//Ctrl Z undo
		INFO("UNDO");
		SaveStateHandler::PrevState();
	} });

	_shortcuts.AddShortCut({ KEY_W, true, true,[]() {//Ctrl Shift Z redo
		INFO("REDO");
		SaveStateHandler::NextState();
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
