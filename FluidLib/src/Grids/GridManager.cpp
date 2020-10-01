#include "GridManager.h"

namespace FluidLib {

    void GridManager::AddGrid(GridBase* grid)
    {
        _grids.push_back(grid);
    }
    void GridManager::RemoveGrid(GridBase* grid)
    {
        //Todo remove grid
    }
    void GridManager::BindGrids()
    {
        for (GridBase * grid : _grids) {
            grid->UseGrid();
        } 
    }

    void GridManager::RenderGrids()
    {
        for (GridBase* grid : _grids) {
            grid->DrawGrid();
        }
    }

}