#pragma once

#include <cmath>

namespace FluidLib {

	class FPoint;

	class IPoint
	{
	public:
		IPoint();
		IPoint(int x, int y);
		IPoint(const IPoint& p);
		IPoint(const FPoint& p);

		void operator=(const IPoint& p);
		void operator=(const FPoint& p);
		IPoint operator+(const IPoint& p2);
		IPoint operator+(const FPoint& p2);
		IPoint operator-(const IPoint& p2);
		IPoint operator-(const FPoint& p2);
		IPoint operator*(int i);
		IPoint operator/(int i);
		void operator+=(const IPoint& p2);
		void operator-=(const IPoint& p2);
		void operator*=(int i);
		void operator/=(int i);

		friend IPoint operator*(float f, const IPoint& p);
		friend IPoint operator/(float f, const IPoint& p);

		inline int GetX() const { return _x; }
		inline int GetY() const { return _y; }
		inline void SetX(int x) { _x = x; }
		inline void SetY(int y) { _y = y; }

		inline int* GetXPtr() { return &_x; }
		inline int* GetYPtr() { return &_y; }

		IPoint& Rotate(float r);
	
	private:
		int _x, _y;
		friend FPoint;
	};

	class FPoint
	{
	public:
		FPoint();
		FPoint(float x, float y);
		FPoint(const FPoint& p);
		FPoint(const IPoint& p);

		void operator=(const FPoint& p);
		void operator=(const IPoint& p);
		FPoint operator+(const FPoint& p2);
		FPoint operator+(const IPoint& p2);
		FPoint operator-(const FPoint& p2);
		FPoint operator-(const IPoint& p2);
		FPoint operator*(float i);
		FPoint operator/(float i);
		void operator+=(const FPoint& p2);
		void operator-=(const FPoint& p2);
		void operator*=(float i);
		void operator/=(float i);

		friend FPoint operator*(float f, const FPoint& p);
		friend FPoint operator/(float f, const FPoint& p);

		inline float GetX() const { return _x; }
		inline float GetY() const { return _y; }
		inline void SetX(float x) {_x = x;}
		inline void SetY(float y) {_y = y;}
		inline void Set(float x, float y) { _x = x; _y = y; }
		inline float* GetXPtr() { return &_x; }
		inline float* GetYPtr() { return &_y; }
		
		FPoint& Rotate(float r);
	private:
		float _x, _y;
		friend IPoint;
	};


}