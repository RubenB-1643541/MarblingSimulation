#pragma once

#include "../Movement.h"

namespace FluidLib {

	class MouseMovement : public Movement
	{
	public:

		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return pair<float, float>: the same mouse position
		*/
		inline virtual FPoint Get(float x, float y) { return { x, y }; }
	};

}