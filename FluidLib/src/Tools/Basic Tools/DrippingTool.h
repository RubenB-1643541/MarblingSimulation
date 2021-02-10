#pragma once

#include "BasicTool.h"
#include <random>

namespace FluidLib {

	class DrippingTool : public BasicTool
	{
	public:
		DrippingTool();
		void OnUse() override;
		float GetPercentage() { return _perc; }
		void SetPercentage(float perc) { _perc = perc; }
		float* GetPercentagePtr() { return &_perc; }
	private:
		float _perc = 0.5;
	};

}