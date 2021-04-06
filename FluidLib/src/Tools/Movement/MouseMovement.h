#pragma once

#include "../Movement.h"
#include <cmath>
#include "corecrt_math_defines.h"
#define _USE_MATH_DEFINES

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
		inline virtual float GetNormalRotation(float x, float y) {
			float newang = atan2(y - _prevy, x - _prevx) + M_PI / 2;
			float dif = _rotation - newang;
			if (dif < 0) {
				if (dif < -M_PI)
					_rotation -= 0.05;
				else
					_rotation += 0.05;
			}
			else {
				if (dif > M_PI)
					_rotation += 0.1;
				else
					_rotation -= 0.1;
			}
			if (_rotation > 2 * M_PI)
				_rotation = 0;
			if (_rotation < 0)
				_rotation = 2 * M_PI;
			_prevx = x;
			_prevy = y;
			return _rotation;
		}
	private:
		float _rotation = 0;
		float _prevx = 0;
		float _prevy = 0;
	};

}