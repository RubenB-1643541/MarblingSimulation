#include "Point.h"

namespace FluidLib {

	IPoint::IPoint() : _x(0), _y(0)
	{
	}

	IPoint::IPoint(int x, int y) : _x(x), _y(y)
	{
	}

	IPoint::IPoint(const IPoint& p) : _x(p._x), _y(p._y)
	{
	}

	IPoint::IPoint(const FPoint& p) : _x(p._x), _y(p._y)
	{
	}

	void IPoint::operator=(const IPoint& p)
	{
		_x = p._x;
		_y = p._y;
	}

	void IPoint::operator=(const FPoint& p)
	{
		_x = p._x;
		_y = p._y;
	}

	IPoint IPoint::operator+(const IPoint& p2)
	{
		return IPoint(_x + p2._x, _y + p2._y);
	}

	IPoint IPoint::operator+(const FPoint& p2)
	{
		return IPoint(_x + p2._x, _y + p2._y);
	}

	IPoint IPoint::operator-(const IPoint& p2)
	{
		return IPoint(_x - p2._x, _y - p2._y );
	}

	IPoint IPoint::operator-(const FPoint& p2)
	{
		return IPoint(_x - p2._x, _y - p2._y);
	}

	IPoint IPoint::operator*(int i)
	{
		return IPoint(_x * i, _y * i );
	}

	IPoint IPoint::operator/(int i)
	{
		return IPoint(_x / i, _y / i );
	}

	void IPoint::operator+=(const IPoint& p2)
	{
		_x += p2._x;
		_y += p2._y;
	}

	void IPoint::operator-=(const IPoint& p2)
	{
		_x -= p2._x;
		_y -= p2._y;
	}

	void IPoint::operator*=(int i)
	{
		_x *= i;
		_y *= i;
	}

	void IPoint::operator/=(int i)
	{
		_x /= i;
		_y /= i;
	}

	FPoint::FPoint() : _x(0.0f), _y(0.0f)
	{
	}

	FPoint::FPoint(float x, float y) : _x(x), _y(y)
	{
	}

	FPoint::FPoint(const FPoint& p) : _x(p._x), _y(p._y)
	{
	}

	FPoint::FPoint(const IPoint& p) : _x(p._x), _y(p._y)
	{
	}

	void FPoint::operator=(const FPoint& p)
	{
		_x = p._x;
		_y = p._y;
	}

	void FPoint::operator=(const IPoint& p)
	{
		_x = p._x;
		_y = p._y;
	}

	FPoint FPoint::operator+(const FPoint& p2)
	{
		return FPoint( _x + p2._x, _y + p2._y );
	}

	FPoint FPoint::operator+(const IPoint& p2)
	{
		return FPoint(_x + p2._x, _y + p2._y);
	}

	FPoint FPoint::operator-(const FPoint& p2)
	{
		return FPoint(_x - p2._x, _y - p2._y );
	}

	FPoint FPoint::operator-(const IPoint& p2)
	{
		return FPoint(_x - p2._x, _y - p2._y);
	}

	FPoint FPoint::operator*(float i)
	{
		return FPoint(_x * i, _y * i );
	}

	FPoint FPoint::operator/(float i)
	{
		return FPoint(_x / i, _y / i );
	}

	void FPoint::operator+=(const FPoint& p2)
	{
		_x += p2._x;
		_y += p2._y;
	}

	void FPoint::operator-=(const FPoint& p2)
	{
		_x -= p2._x;
		_y -= p2._y;
	}

	void FPoint::operator*=(float i)
	{
		_x *= i;
		_y *= i;
	}

	void FPoint::operator/=(float i)
	{
		_x /= i;
		_y /= i;
	}

	IPoint operator*(float f, const IPoint& p)
	{
		return IPoint(f * p._x, f * p._y);
	}

	IPoint operator/(float f, const IPoint& p)
	{
		return IPoint(p._x / f, p._y / f);
	}

	FPoint operator*(float f, const FPoint& p)
	{
		return FPoint(f * p._x, f * p._y);
	}

	FPoint operator/(float f, const FPoint& p)
	{
		return FPoint(p._x / f, p._y / f);
	}

}