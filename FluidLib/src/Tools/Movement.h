#pragma once

#include <utility>
#include <string>
#include "glm/gtx/transform.hpp"
#include "Util/Point.h"

namespace FluidLib {

	class Movement
	{
	public:

	
		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint new position
		*/
		inline virtual FPoint Get(float x, float y) { return {x, y}; }

		inline virtual void DrawPath() {}
		inline virtual bool OnScroll(float x, float y) { return false; }
		inline void SetProjection(glm::mat4 proj) { _projection = proj; }
		std::string& GetType() { return _type; }
	protected:
		glm::mat4 _projection = glm::mat4(1.0);
		std::string _type = "None";
	};

}