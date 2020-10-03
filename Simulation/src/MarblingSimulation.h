#pragma once

#include "Simulation.h"
#include "Grids/Grid.h"
#include "GridStructures.h"

class MarblingSimulation : public FluidLib::Simulation
{
public:
	MarblingSimulation(int sizex, int sizey);

	void OnInit() override;
	void OnUpdate() override;
	void OnDraw() override;

private:



};

