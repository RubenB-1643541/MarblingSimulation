#include "SaveStateHandler.h"



char* SaveStateHandler::_loc = "Savestates\\";
std::string SaveStateHandler::_curloc = "";
int SaveStateHandler::_counter = 0;
std::vector<State> SaveStateHandler::_states;
std::filesystem::directory_iterator SaveStateHandler::_fileit;

void SaveStateHandler::Init()
{
	struct stat info;
	if (stat(_loc, &info) != 0) {
		INFO("DIR DOES NOT EXISTS");
		_mkdir(_loc);
	}
	int h = 0;
	for (const auto& entry : std::filesystem::directory_iterator(_loc)) {
		std::string file = entry.path().generic_string();
		std::string state = file;
		state = state.substr(state.find("/")).erase(0, 1);
		int n = std::stoi(state);
		if (n >= h)
			h = n;
	}
	++h;
	_curloc = _loc + std::to_string(h) + "\\";
	_mkdir(_curloc.c_str());
	std::filesystem::path path = _curloc;
	_curloc = std::filesystem::current_path().u8string();
	_curloc += std::string("\\") + _loc + std::to_string(h) + "\\";
	std::cout << _curloc << std::endl;
}

State SaveStateHandler::CreateSaveState()
{
	++_counter;
	std::string file = _curloc + std::to_string(_counter) + ".s";
	std::ofstream stream;
	stream.open(file.c_str(), std::ios::binary);
	FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink")->WriteToFile(stream, true);
	stream.close();
	State s = { _counter, file };
	
	return s;
}

std::vector<State>& SaveStateHandler::GetStates()
{
	_states.clear();
	try {
		for (const auto& entry : std::filesystem::directory_iterator(_curloc)) {
			std::string file = entry.path().generic_string();
			std::string state = file;
			state = state.substr(state.find_last_of("/")).erase(0, 1);
			//state = state.substr(state.find("/")).erase(0, 1);
			int n = std::stoi(state.erase(state.find(".")));
			_states.push_back({ n, file });
			//files.push_back(state.substr(state.find("/")).erase(0,1));
		}
	}
	catch (std::filesystem::filesystem_error e) {
		std::cerr << "Filesystem error at savestates" << e.what() << std::endl;
	}
	std::sort(_states.begin(), _states.end());
	return _states;
}

bool SaveStateHandler::LoadState(const State& state)
{
	FluidLib::Grid<IInk>* grid = static_cast<FluidLib::Grid<IInk>*>(FluidLib::Simulation::Get()->GetGrids()->GetGrid("Ink"));
	IInk* inks = (IInk*)grid->GetBufferPointer();
	std::ifstream stream;
	stream.open(state.file, std::ios::binary);
	stream.read((char*)inks, sizeof(IInk) * grid->GetBufferData().size);
	grid->ReleaseBufferPointer();
	stream.close();
	_counter = state.n;
	return true;
}

int SaveStateHandler::GetCurrentStateNumber()
{
	return _counter;
}

bool SaveStateHandler::PrevState()
{
	if (_counter <= 1)
		return false;
	--_counter;
	LoadState(_states[_counter-1]);
	return true;
}

bool SaveStateHandler::NextState()
{
	if (_counter >= _states.size())
		return false;
	++_counter;
	LoadState(_states[_counter - 1]);
	return true;
}

std::string& SaveStateHandler::GetLocation()
{
	return _curloc;
}

void SaveStateHandler::SetLocation(const std::string& loc, bool remove)
{
	if(remove)
		rmdir(_curloc.c_str());
	_curloc = loc;
	struct stat info;
	if (stat(_curloc.c_str(), &info) != 0) {
		INFO("DIR DOES NOT EXISTS");
		_mkdir(_curloc.c_str());
	}
	GetStates();
	_counter = _states.size();
}

bool State::operator<(const State& s2)
{
	return n < s2.n;
}

bool State::operator>(const State& s2)
{
	return n > s2.n;
}
