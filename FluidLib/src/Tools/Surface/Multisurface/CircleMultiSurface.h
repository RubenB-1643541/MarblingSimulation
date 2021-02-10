#pragma once

#include "MultiSurface.h"
#include <cmath>
#include "corecrt_math_defines.h"
#define _USE_MATH_DEFINES

namespace FluidLib {

	class CircleMultiSurface : public MultiSurface
	{
	public:
		CircleMultiSurface();
		virtual std::vector<FPoint>& GetPoints() override;

		float GetR() { return _r; }
		void SetR(float r) { _r = r; }
		float* GetRPtr() { return &_r; }

		int GetRepetition() { return _rep; }
		void SetRepetition(int rep) { _rep = rep; }
		int* GetRepetitionPtr() { return &_rep; }

		float GetOffset() { return _offset; }
		void SetOffset(float offset) { _offset = offset; }
		float* GetOffsetPtr() { return &_offset; }
	private:
		float _r = 50;
		float _rold;
		int _rep = 5;
		int _repold;
		float _offset = -M_PI/2;
		//float _offset = 2*M_PI/3;
		float _offsetold;
	};

}