#pragma once

#include "Parameters.h"
#include <utility>
#include <string>
#include "glm/gtx/transform.hpp"
#include "Util/Point.h"
#include <cmath>

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
		inline virtual float GetNormalRotation(float x, float y) { return 0.0f; }

		inline virtual void StartEdit() {}
		inline virtual void DrawPath() {}
		inline virtual void OnEditDraw() {}
		inline virtual bool OnScroll(float x, float y) { return false; }
		inline virtual bool OnEditClick(float x, float y) { return false; }
		inline virtual bool OnEditRelease(float x, float y) { return false; }
		inline virtual bool OnEditMove(float x, float y) { return false; }
		inline virtual void SetProjection(glm::mat4 proj) { _projection = proj; }
		std::string& GetType() { return _type; }

		virtual void LoadParam(Parameters* p) = 0;
	protected:
		glm::mat4 _projection = glm::mat4(1.0);
		std::string _type = "None";
	};

}