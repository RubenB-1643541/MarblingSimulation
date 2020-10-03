#pragma once

#include "Grid.h"
#include "../Events/Events.h"
#include <vector>

namespace FluidLib {

	class GridManager
	{
	public:

		void OnEvent(Event& event);
		//Add Grid
		//Remove Grid
		//UpdateGrid
		void AddGrid(GridBase* grid);
		void RemoveGrid(GridBase* grid);
		void BindGrids();
		void RenderGrids();

		std::vector<GridBase*>::iterator begin() { return _grids.begin(); }
		std::vector<GridBase*>::iterator end() { return _grids.end(); }
	private:
		std::vector<GridBase*> _grids;

	};

}