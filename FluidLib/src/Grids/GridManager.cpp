#include "GridManager.h"

namespace FluidLib {
    
    void GridManager::BindGrids()
    {
        for (GridBase * grid : _grids) {
            grid->UseGrid();
        } 
    }

}