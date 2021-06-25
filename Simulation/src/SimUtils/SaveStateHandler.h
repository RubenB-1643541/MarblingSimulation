#pragma once
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include "Tools/ToolManager.h"
#include "Simulation.h"
#include "../SimulationCore/GridStructures.h"
#include <sys/stat.h>
#include "Core/Log.h"
#include <direct.h>

struct State {
	int n;
	std::string file;
	bool operator<(const State& s2);
	bool operator>(const State& s2);
};

class SaveStateHandler
{
public:
	static void Init();
	static State CreateSaveState();
	static std::vector<State>& GetStates();
	static bool LoadState(const State& state);
	static int GetCurrentStateNumber();
	static bool PrevState();
	static bool NextState();
	static std::string& GetLocation();
	static void SetLocation(const std::string& loc, bool remove = false);
	
private:
	static char* _loc;
	static std::string _curloc;
	static int _counter;
	static std::vector<State> _states;
	static std::filesystem::directory_iterator _fileit;

};

