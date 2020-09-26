#include "SimulationApplication.h"
#include "GridStructures.h"

SimulationApplication::SimulationApplication() : Application("Marbling Simulation", new SimulationWindow()), _sim(100,100)
{
	_buffers.clear();
	RenderEngine::ShaderStorageBuffer* velbuf = new RenderEngine::ShaderStorageBuffer();
	velbuf->Bind();
	velbuf->BufferData(_sim.GetSize() * sizeof(IVelocity));
	IVelocity* vels = (IVelocity*)velbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		vels[i] = { 0 ,0 };
	}
	velbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Vel", velbuf));

	RenderEngine::ShaderStorageBuffer* freqbuf = new RenderEngine::ShaderStorageBuffer();
	freqbuf->Bind();
	freqbuf->BufferData(_sim.GetSize() * sizeof(IFrequency));
	IFrequency * freqs = (IFrequency*)freqbuf->MapBufferRange();
	for (int i = 0; i < _sim.GetSize(); ++i) {
		freqs[i] = { 0 };
	}
	freqbuf->UnMapBuffer();
	_buffers.insert(std::make_pair("Freq", freqbuf));

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

	_computeshader.Create("res/computeshaders/final_computeshader.glsl");
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
	gridman->AddGrid(velgrid);

	FluidLib::Grid<IFrequency>* freqgrid = new FluidLib::Grid<IFrequency>(_buffers.at("Freq")->GetId(), _sim.GetSize(), 2);
	gridman->AddGrid(freqgrid);

	FluidLib::Grid<IInk>* inkgrid = new FluidLib::Grid<IInk>(_buffers.at("Ink")->GetId(), _sim.GetSize(), 3);
	gridman->AddGrid(inkgrid);

	FluidLib::Grid<Flags>* flaggrid = new FluidLib::Grid<Flags>(_buffers.at("Flag")->GetId(), _sim.GetSize(), 4);
	gridman->AddGrid(flaggrid);

	FluidLib::ComputeShaderController * shadercontroller = _sim.GetShader();
	shadercontroller->SetShader(_computeshader.GetId());
	FluidLib::UniformVal widthval; widthval.intval = _sim.GetSizeX();
	FluidLib::UniformVal heightval; heightval.intval = _sim.GetSizeY();
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, widthval, "Width"));
	shadercontroller->AddUniform(FluidLib::Uniform(FluidLib::UniformType::INT, heightval, "Height"));

	_sim.Init();
}

void SimulationApplication::OnStop()
{
	_sim.Stop();
}

void SimulationApplication::OnUpdate()
{
	_sim.Update();

	RenderEngine::ShaderStorageBuffer* freqbuf = _buffers.at("Freq");
	freqbuf->Bind();
	IFrequency* freqs = (IFrequency*)freqbuf->MapBufferRange();
	INFO(freqs[0].freq);
	freqbuf->UnMapBuffer();
}
