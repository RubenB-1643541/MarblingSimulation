#pragma once

#include "Simulation.h"
#include "Grids/Grid.h"
#include "GridStructures.h"

class MarblingSimulation : public FluidLib::Simulation
{
public:
	MarblingSimulation(int sizex, int sizey);

	void OnInit() override;
	void BeforeUpdate() override;
	void AfterUpdate() override;
	void OnDraw() override;
	inline FluidLib::ToolManager* GetTools() { return &_tools; }

private:
	void CreateTools();


};

