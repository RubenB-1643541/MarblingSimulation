#pragma once
namespace FluidLib {

	class IPoint
	{
	public:
		IPoint();
		IPoint(int x, int y);
		IPoint(const IPoint& p);

		void operator=(const IPoint& p);
		IPoint operator+(const IPoint& p2);
		IPoint operator-(const IPoint& p2);
		IPoint operator*(int i);
		IPoint operator/(int i);
		void operator+=(const IPoint& p2);
		void operator-=(const IPoint& p2);
		void operator*=(int i);
		void operator/=(int i);

		inline int GetX() const { return _x; }
		inline int GetY() const { return _y; }
	
	private:
		int _x, _y;
	};

	class FPoint
	{
	public:
		FPoint();
		FPoint(float x, float y);
		FPoint(const FPoint& p);

		void operator=(const FPoint& p);
		FPoint operator+(const FPoint& p2);
		FPoint operator-(const FPoint& p2);
		FPoint operator*(int i);
		FPoint operator/(int i);
		void operator+=(const FPoint& p2);
		void operator-=(const FPoint& p2);
		void operator*=(int i);
		void operator/=(int i);

		inline float GetX() const { return _x; }
		inline float GetY() const { return _y; }
	
	private:
		float _x, _y;
	};


}