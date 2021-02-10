#include "CircleMultiSurface.h"

namespace FluidLib {

	CircleMultiSurface::CircleMultiSurface()
	{
		_rold = _r;
		_repold = _rep;
		_offsetold = _offset;
		_changed = true;
	}

	std::vector<FPoint>& CircleMultiSurface::GetPoints()
	{
		if (_rold != _r)
			_changed = true;
		if (_repold != _rep)
			_changed = true;
		if (_offsetold != _offset)
			_changed = true;
		if (_changed) {
			_rold = _r;
			_repold = _rep;
			_offsetold = _offset;
			_changed = false;
			_points.clear();
			float perc = 1.0f / _rep;
			for (float i = 0; i < 1; i += perc) {
				float theta = 2 * M_PI * i;
				_points.push_back({ _r * cosf(theta + _offset), _r * sinf(theta + _offset) });
			}
		}
		return _points;
	}

}