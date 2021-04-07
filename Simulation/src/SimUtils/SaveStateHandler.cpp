#include "SaveStateHandler.h"
#include <sys/stat.h>
#include "Core/Log.h"
#include <direct.h>


char* SaveStateHandler::_loc = "Savestates\\";
std::string SaveStateHandler::_curloc = "";
int SaveStateHandler::_counter = 0;
std::vector<State> SaveStateHandler::_states;

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
	for (const auto& entry : std::filesystem::directory_iterator(_curloc.c_str())) {
		std::string file = entry.path().generic_string();
		std::string state = file;
		state = state.substr(state.find("/")).erase(0, 1);
		state = state.substr(state.find("/")).erase(0, 1);
		int n = std::stoi(state.erase(state.find(".")));
		_states.push_back({ n, file });
		//files.push_back(state.substr(state.find("/")).erase(0,1));
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
