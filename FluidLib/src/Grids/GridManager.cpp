#include "GridManager.h"

namespace FluidLib {
    void GridManager::OnEvent(Event& event)
    {
    }
    void GridManager::AddGrid(const std::string& name, GridBase* grid)
    {
        _grids.insert(std::make_pair(name, grid));
    }
    void GridManager::RemoveGrid(const std::string& name)
    {
        _grids.erase(name);
        //Todo remove grid
    }
    GridBase* GridManager::GetGrid(const std::string& name)
    {
        if(_grids.find(name) != _grids.end())
            return _grids[name];
        return nullptr;
    }
    void GridManager::BindGrids()
    {
        for (std::pair<std::string, GridBase *> grid : _grids) {
            grid.second->UseGrid();
        } 
    }

    void GridManager::RenderGrids()
    {
        for (std::pair<std::string, GridBase*> grid : _grids) {
            grid.second->DrawGrid();
        }
    }

}