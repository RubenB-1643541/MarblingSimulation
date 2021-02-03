#include "MultiSurface.h"

namespace FluidLib {

    std::vector<FPoint>& MultiSurface::GetPoints()
    {
        if (_changed)
            _changed = false;
        return _points;
    }

}