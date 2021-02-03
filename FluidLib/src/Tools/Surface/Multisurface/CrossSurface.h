#pragma once

#include "MultiSurface.h"

namespace FluidLib {

	class CrossSurface : public MultiSurface
	{
	public:
		CrossSurface();
		virtual std::vector<FPoint>& GetPoints() override;

		inline float GetLen() { return _len; }
		inline void SetLen(float len) { _len = len; }
		inline float* GetLenPtr() { return &_len; }

		inline bool GetX() { return _x; }
		inline void SetX(bool x) { _x = x; }
		inline bool* GetXPtr() { return &_x; }
	private:
		float _len = 10.0f;
		float _lenold = 10.0f;

		bool _x = false;
		bool _xold;
	};

}