#pragma once

#include <utility>

namespace FluidLib {

	class Movement
	{
	public:

		virtual void Init() {}

		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return pair<float, float>: new position
		*/
		inline virtual FPoint Get(float x, float y) { return {x, y}; }

		inline virtual void DrawPath() {}
		inline virtual void OnScroll(float x, float y) {}
	};

}