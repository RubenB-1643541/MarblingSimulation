#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Grids/GridManager.h"
#include "Core/Log.h"
#include "Simulation.h"
#include "Grids/Grid.h"
#include "Rendering/ShaderStorageBuffer.h"
#include "../SimulationCore/GridStructures.h"
/*
Save Grids
-> Grid Size
-> Grid Name
-> Grid Data
-> Grid Data (Type * Size)
*/

class SimSave
{
public:

	bool Save();
	void SetFile(const std::string& file);
	void SetGrids(FluidLib::GridManager* grids);
private:
	bool StartSave();
	bool SaveSimData();
	bool SaveGrid(const std::string& name, FluidLib::GridBase * grid);
	std::string _file;
	std::ofstream _ostream;
	FluidLib::GridManager* _grids;
};

class SimLoad
{
public:
	void SetFile(const std::string& file);
	void SetGrids(FluidLib::GridManager* grids);
	void SetBuffers(std::map<std::string, RenderEngine::ShaderStorageBuffer*>* buffers);
	bool StartLoad();
	bool LoadSimData();
	bool LoadGrids();
	bool EndLoad();
private:
	bool LoadGrid();

	std::string _file;
	std::ifstream _istream;
	FluidLib::GridManager* _grids;
	std::map<std::string, RenderEngine::ShaderStorageBuffer*>* _buffers;


};

