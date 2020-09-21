#pragma once

#include "Grid.h"
#include <vector>

namespace FluidLib {

	class GridManager
	{
	public:


		//Add Grid
		//Remove Grid
		//UpdateGrid
		void BindGrids();

		std::vector<GridBase*>::iterator begin() { return _grids.begin(); }
		std::vector<GridBase*>::iterator end() { return _grids.end(); }
	private:
		std::vector<GridBase*> _grids;

	};

}