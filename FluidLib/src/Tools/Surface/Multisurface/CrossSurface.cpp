#include "CrossSurface.h"

namespace FluidLib {

    CrossSurface::CrossSurface()
    {
        _points.clear();
        _points.push_back({ 0.0f, 0.0f });
        _points.push_back({ _len, _len });
        _points.push_back({ _len, -_len });
        _points.push_back({ -_len, _len });
        _points.push_back({ -_len, -_len });
        _changed = false;
        _lenold = _len;
        _xold = _x;
    }

    std::vector<FPoint>& CrossSurface::GetPoints()
    {
        if (_len != _lenold)
            _changed = true;
        if (_x != _xold)
            _changed = true;
        if (_changed) {
            _lenold = _len;
            _xold = _x;
            _changed = false;
            _points.clear();
            _points.push_back({ 0.0f, 0.0f });
            if (_x) {
                _points.push_back({ _len, _len });
                _points.push_back({ _len, -_len });
                _points.push_back({ -_len, _len });
                _points.push_back({ -_len, -_len });
            }
            else
            {
                _points.push_back({ 0, _len });
                _points.push_back({ 0, -_len});
                _points.push_back({ _len, 0 });
                _points.push_back({ -_len, 0 });
            }
            
        }
        return _points;
    }
}