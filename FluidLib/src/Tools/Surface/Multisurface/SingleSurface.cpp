#include "SingleSurface.h"

namespace FluidLib {

	SingleSurface::SingleSurface()
	{
		_points.clear();
		_points.push_back({ 0.0f, 0.0f });
		_changed = false;
	}

	std::vector<FPoint>& SingleSurface::GetPoints()
	{
		return _points;
	}

}