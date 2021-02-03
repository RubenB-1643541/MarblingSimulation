#include "Comb.h"

namespace FluidLib {



	Comb::Comb()
	{

		_lenold = _len;
		_repold = _rep;
		_vertold = _vert;
	}

	std::vector<FPoint>& Comb::GetPoints()
	{
		if (_len != _lenold)
			_changed = true;
		if (_rep != _repold)
			_changed = true;
		if (_vert != _vertold)
			_changed = true;
		if (_changed) {
			_lenold = _len;
			_repold = _rep;
			_vertold = _vert;
			_changed = false;
			_points.clear();
			for (int i = -_rep+1; i < _rep; ++i) {
				float x = 0;
				float y = 0;
				if (_vert) {
					y = _len * i / _rep;
				}
				else {
					x = _len * i / _rep;
				}
				_points.push_back({ x,y });
			}
		}
		return _points;
	}

}
