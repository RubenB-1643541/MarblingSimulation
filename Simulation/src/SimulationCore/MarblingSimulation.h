#pragma once

#include "Simulation.h"
#include "Grids/Grid.h"
#include "GridStructures.h"
#include "Tools/Basic Tools/BasicTool.h"
#include "../SimInterface/ToolSelectComponent.h"

class MarblingSimulation : public FluidLib::Simulation
{
public:
	MarblingSimulation();
	MarblingSimulation(int sizex, int sizey);

	void OnInit() override;
	void BeforeUpdate() override;
	void AfterUpdate() override;
	void OnDraw() override;
	inline FluidLib::ToolManager* GetTools() { return &_tools; }
	void InitBasicToolComponent(ToolSelectComponent* comp);

private:
	void CreateTools();
	void CreateBasicTool();


};

