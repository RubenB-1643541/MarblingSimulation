#pragma once

#include "MultiSurface.h"

namespace FluidLib {

	class SingleSurface : public MultiSurface
	{
	public:
		SingleSurface();
		virtual std::vector<FPoint>& GetPoints() override;
	private:


	};

}