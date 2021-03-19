#pragma once

#include "Simulation.h"
#include "Grids/Grid.h"
#include "Grids/FlagGrid.h"
#include "GridStructures.h"
#include "Tools/Basic Tools/BasicTool.h"
#include "../SimInterface/ToolSelectComponent.h"
#include "Tools/Basic Tools/SelectTool.h"
#include "Tools/Basic Tools/DrippingTool.h"
#include "Tools/Basic Tools/FanTool.h"
#include "../SimUtils/Icon.h"

class MarblingSimulation : public FluidLib::Simulation
{
public:
	MarblingSimulation();
	MarblingSimulation(int sizex, int sizey);

	void OnInit() override;
	void OnUpdate() override;
	void BeforeUpdate() override;
	void AfterUpdate() override;
	void OnDraw() override;
	
	void InitBasicToolComponent(ToolSelectComponent* comp);

private:
	void CreateTools();
	void CreateBasicTool();

	std::clock_t _prevtime;


};

