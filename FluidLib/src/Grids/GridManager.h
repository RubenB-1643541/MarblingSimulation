#pragma once

#include "Grid.h"
#include "../Events/Events.h"
#include <map>

namespace FluidLib {

	class GridManager
	{
	public:

		void OnEvent(Event& event);
		//Add Grid
		//Remove Grid
		//UpdateGrid
		void AddGrid(const std::string& name, GridBase* grid);
		void RemoveGrid(const std::string& name);
		GridBase* GetGrid(const std::string& name);
		void BindGrids();
		void RenderGrids();

		std::map<std::string, GridBase*>::iterator begin() { return _grids.begin(); }
		std::map<std::string, GridBase*>::iterator end() { return _grids.end(); }
	private:
		std::map<std::string, GridBase*> _grids;

	};

}