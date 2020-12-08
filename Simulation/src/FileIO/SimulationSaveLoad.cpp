#include "SimulationSaveLoad.h"

bool SimSave::Save()
{
	if (!StartSave()) {
		ERROR("Failed to open file : ");
		ERROR(_file.c_str());
		return false;
	}
	if (!SaveSimData()) {
		ERROR("Failed To Save Simulation Data");
		return false;
	}
	for (auto grid : *_grids) {
		SaveGrid(grid.first, grid.second);
	}
	
	_ostream.close();
	INFO("Save Done");
	return true;
}

void SimSave::SetFile(const std::string& file)
{
	_file = file;
}

void SimSave::SetGrids(FluidLib::GridManager* grids)
{
	_grids = grids;
}

bool SimSave::StartSave()
{
	//Open File
	_ostream.open(_file, std::ios::out | std::ios::trunc);
	_ostream.close();
	_ostream.open(_file, std::ios::out | std::ios::app);
	
	return _ostream.is_open();
}

bool SimSave::SaveSimData()
{
	//Save Grid Size
	_ostream << FluidLib::Simulation::Get()->GetSizeX() << " " << FluidLib::Simulation::Get()->GetSizeY() << std::endl;
	//Save Settings
	return true;
}

bool SimSave::SaveGrid(const std::string& name, FluidLib::GridBase* grid)
{
	if (name.find("2") != std::string::npos) {
		INFO("CopyBuffer");
		return false;
	}
	_ostream << name << std::endl;
	//Buffer Data
	FluidLib::BufferData data = grid->GetBufferData();
	_ostream << data.id << " " << data.size << " " << data.elementsize << " " << data.attrpointer << " " << data.type << std::endl;
	grid->WriteToFile(_ostream);
	_ostream << std::endl;
	return true;
}

void SimLoad::SetFile(const std::string& file)
{
	_file = file;
}

void SimLoad::SetGrids(FluidLib::GridManager* grids)
{
	_grids = grids;
}

bool SimLoad::Load()
{
	if (!StartLoad()) {
		ERROR("Failed to open file : ");
		ERROR(_file.c_str());
		return false;
	}
	if (!LoadSimData()) {
		ERROR("Failed To Load Simulation Data");
		return false;
	}
	while (!_istream.eof()) {
		LoadGrid();
	}
	//for (auto grid : *_grids) {
	//	LoadGrid(grid.first, grid.second);
	//}

	_istream.close();
	INFO("Load Done");
	return true;
}

void SimLoad::SetBuffers(std::map<std::string, RenderEngine::ShaderStorageBuffer*>* buffers)
{
	_buffers = buffers;
}

bool SimLoad::StartLoad()
{
	_istream.open(_file, std::ios::in);
	return _istream.is_open();
}

bool SimLoad::LoadSimData()
{
	int width, height;
	_istream >> width >> height;
	FluidLib::Simulation::Get()->SetSize(width, height);
	return true;
}

bool SimLoad::LoadGrid()
{
	std::string name;
	_istream >> name;
	//Buffer Data
	FluidLib::BufferData data;
	_istream >> data.id >> data.size >> data.elementsize >> data.attrpointer >> data.type;
	if (name == "Vel") {
		RenderEngine::ShaderStorageBuffer* velbuf = new RenderEngine::ShaderStorageBuffer();
		data.id = velbuf->GetId();
		velbuf->Bind();
		velbuf->BufferData(data.size * sizeof(IVelocity));
		IVelocity* vels = (IVelocity*)velbuf->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			_istream >> vels[i];
		}
		velbuf->UnMapBuffer();
		if(_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), velbuf));
		FluidLib::Grid<IVelocity>* velgrid = new FluidLib::Grid<IVelocity>(data.id, data.size, data.attrpointer);
		velgrid->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), velgrid);
		name.append("2");
		RenderEngine::ShaderStorageBuffer* velbuf2 = new RenderEngine::ShaderStorageBuffer();
		data.id = velbuf2->GetId();
		velbuf2->Bind();
		velbuf2->BufferData(data.size * sizeof(IVelocity));
		IVelocity* vels2 = (IVelocity*)velbuf2->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			vels2[i] = { 0, 0 };
		}
		velbuf2->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), velbuf));
		FluidLib::Grid<IVelocity>* velgrid2 = new FluidLib::Grid<IVelocity>(data.id, data.size, data.attrpointer+1);
		velgrid2->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), velgrid2);

	}
	else if (name == "Freq") {
		RenderEngine::ShaderStorageBuffer* freqbuf = new RenderEngine::ShaderStorageBuffer();
		data.id = freqbuf->GetId();
		freqbuf->Bind();
		freqbuf->BufferData(data.size * sizeof(IFrequency));
		IFrequency* freqs = (IFrequency*)freqbuf->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			_istream >> freqs[i];
		}
		freqbuf->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), freqbuf));
		FluidLib::Grid<IFrequency>* freqgrid = new FluidLib::Grid<IFrequency>(data.id, data.size, data.attrpointer);
		freqgrid->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), freqgrid);

		RenderEngine::ShaderStorageBuffer* freqbuf2 = new RenderEngine::ShaderStorageBuffer();
		name.append("2");
		data.id = freqbuf2->GetId();
		freqbuf2->Bind();
		freqbuf2->BufferData(data.size * sizeof(IFrequency));
		IFrequency* freqs2 = (IFrequency*)freqbuf2->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			freqs2[i] = { 0 };
		}
		freqbuf2->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), freqbuf2));
		FluidLib::Grid<IFrequency>* freqgrid2 = new FluidLib::Grid<IFrequency>(data.id, data.size, data.attrpointer);
		freqgrid2->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), freqgrid2);
	}
	else if (name == "Ink") {
		RenderEngine::ShaderStorageBuffer* inkbuf = new RenderEngine::ShaderStorageBuffer();
		data.id = inkbuf->GetId();
		inkbuf->Bind();
		inkbuf->BufferData(data.size * sizeof(IInk));
		IInk* inks = (IInk*)inkbuf->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			_istream >> inks[i];
		}
		inkbuf->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), inkbuf));
		FluidLib::Grid<IInk>* inkgrid = new FluidLib::Grid<IInk>(data.id, data.size, data.attrpointer+1);
		inkgrid->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), inkgrid);

		RenderEngine::ShaderStorageBuffer* inkbuf2 = new RenderEngine::ShaderStorageBuffer();
		data.id = inkbuf2->GetId();
		name.append("2");
		inkbuf2->Bind();
		inkbuf2->BufferData(data.size * sizeof(IInk));
		IInk* inks2 = (IInk*)inkbuf2->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			inks2[i] = { 0, 0, {0,0}, {0,0,0},0 };
		}
		inkbuf2->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), inkbuf2));
		FluidLib::Grid<IInk>* inkgrid2 = new FluidLib::Grid<IInk>(data.id, data.size, data.attrpointer+1);
		inkgrid2->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), inkgrid2);
	}
	else if (name == "Flag") {
		RenderEngine::ShaderStorageBuffer* flagbuf = new RenderEngine::ShaderStorageBuffer();
		data.id = flagbuf->GetId();
		flagbuf->Bind();
		flagbuf->BufferData(data.size * sizeof(Flags));
		Flags* flags = (Flags*)flagbuf->MapBufferRange();
		for (int i = 0; i < data.size; ++i) {
			_istream >> flags[i];
		}
		flagbuf->UnMapBuffer();
		if (_buffers != nullptr)
			_buffers->insert(std::make_pair(name.c_str(), flagbuf));
		FluidLib::Grid<Flags>* flaggrid = new FluidLib::Grid<Flags>(data.id, data.size, data.attrpointer);
		flaggrid->SetElementSize(data.elementsize);
		_grids->AddGrid(name.c_str(), flaggrid);
	}
	//grid->WriteToFile(_ostream);
	//_ostream << std::endl;
	return true;
}
