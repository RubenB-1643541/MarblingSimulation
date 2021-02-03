#pragma once

#include "../Movement.h"

namespace FluidLib {

	class MouseMovement : public Movement
	{
	public:
		MouseMovement() { _type = "Mouse"; }
		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint: the same mouse position
		*/
		inline virtual FPoint Get(float x, float y) override { return { x, y }; } 
	};

}