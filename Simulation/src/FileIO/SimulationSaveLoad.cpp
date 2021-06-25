#include "SimulationSaveLoad.h"
#include "../SimUtils/SaveStateHandler.h"

std::string SimSave::_file = "";

bool SimSave::Save()
{
	if (_file == "") {
		_file = FileDialog::Save();
	}
	std::cout << "FILE: " << _file << std::endl;
	if (_file == "")
		return false;
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
	_ostream.open(_file, std::ios::out | std::ios::binary);
	
	return _ostream.is_open();
}

bool SimSave::SaveSimData()
{
	//Save Grid Size
	_ostream << FluidLib::Simulation::Get()->GetSizeX() << " " << FluidLib::Simulation::Get()->GetSizeY() << std::endl;
	//Save Settings
	FluidLib::Settings * settings = FluidLib::Simulation::Get()->GetSettings();
	_ostream << settings->fps << " " << settings->freezeintensity << " " << settings->intesity << " " << settings->diffuse << " " << settings->spreading << std::endl;
	_ostream << SaveStateHandler::GetLocation() << std::endl;
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
	//FluidLib::Grid<Flags>* f = static_cast<FluidLib::Grid<Flags>*>(grid);
	//Flags* d = f->GetBufferPointer();
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

void SimLoad::SetBuffers(std::map<std::string, RenderEngine::ShaderStorageBuffer*>* buffers)
{
	_buffers = buffers;
}

bool SimLoad::StartLoad()
{
	_istream.open(_file, std::ios::in | std::ios::binary);
	return _istream.is_open();
}

bool SimLoad::LoadSimData()
{
	int width, height;
	_istream >> width >> height;
	FluidLib::Simulation::Get()->SetSize(width, height);
	FluidLib::Settings* settings = FluidLib::Simulation::Get()->GetSettings();
	_istream >> settings->fps >> settings->freezeintensity >> settings->intesity >> settings->diffuse >> settings->spreading;
	std::string file;
	_istream >> file;
	SaveStateHandler::SetLocation(file, true);
	INFO("Sim Data Loaded");
	return true;
}

bool SimLoad::LoadGrid()
{
	std::string name;
	_istream >> name;
	
	INFO("Loading {0}", name);
	//Buffer Data
	FluidLib::BufferData data;
	_istream >> data.id >> data.size >> data.elementsize >> data.attrpointer >> data.type;
	if (name == "Vel") {
		_istream.seekg(1, std::ios::cur);//Clear CR and newline (\n)
		_buffers->at(name)->Bind();
		IVelocity* vels = (IVelocity*)_buffers->at(name)->MapBufferRange();
		_istream.read((char*)vels, sizeof(IVelocity) * data.size);
		//for (int i = 0; i < data.size; ++i) {
		//	_istream >> vels[i];
		//}
		_buffers->at(name)->UnMapBuffer();
	}
	else if (name == "Freq") {
		_istream.seekg(1, std::ios::cur);//Clear CR and newline (\n)
		_buffers->at(name)->Bind();
		IFrequency* freqs = (IFrequency*)_buffers->at(name)->MapBufferRange();
		_istream.read((char*)freqs, sizeof(IFrequency) * data.size);
		//for (int i = 0; i < data.size; ++i) {
		//	_istream >> freqs[i];
		//}
		_buffers->at(name)->UnMapBuffer();
	}
	else if (name == "Ink") {
		//Load Colors
		FluidLib::ColorGrid<IInk>* colorgrid = static_cast<FluidLib::ColorGrid<IInk>*>(_grids->GetGrid("Ink"));
		int size;	
		_istream >> size;
		glm::vec3 col;
		colorgrid->ClearColors();
		for (int i = 0; i < size; ++i) {
			_istream >> col.r >> col.g >> col.b;
			colorgrid->AddColor(col);
		}
		
		colorgrid->SetId(colorgrid->GetColors().size()-1);
		_istream.seekg(2, std::ios::cur);//Clear SPACE, CR and newline (\n)
		_buffers->at(name)->Bind();
		INFO("Ink: {0} - size: {1} - tot: {2}", sizeof(IInk), data.size, sizeof(IInk) * data.size);
		IInk* inks = (IInk*)_buffers->at(name)->MapBufferRange();
		_istream.read((char*)inks, sizeof(IInk) * data.size);
		INFO("Read: {}", _istream.peek());
		//for (int i = 0; i < data.size; ++i) {
		//	_istream >> inks[i];
		//}
		_buffers->at(name)->UnMapBuffer();
	}
	else if (name == "Flag") {
		_istream.seekg(1, std::ios::cur);//Clear CR and newline (\n)
		_buffers->at(name)->Bind();
		Flags* flags = (Flags*)_buffers->at(name)->MapBufferRange(GL_MAP_WRITE_BIT);
		INFO("Read: {}", _istream.peek());
		_istream.read((char*)flags, sizeof(Flags) * data.size);
		
		//for (int i = 0; i < data.size; ++i) {
		//	_istream >> flags[i];
		//}
		_buffers->at(name)->UnMapBuffer();
	}
	INFO("LOADED {0}", name);
	INFO("Read: {}", _istream.peek());
	return true;
}

bool SimLoad::LoadGrids()
{
	bool res = true;
	while (!_istream.eof() && res) {
		res = LoadGrid();
	}
	return res;
}

bool SimLoad::EndLoad()
{
	_istream.close();
	INFO("Load Done");
	return false;
}
