#pragma once

#include <vector>
#include <string>
#include "Util/Point.h"
#include "glm/gtx/transform.hpp"

namespace FluidLib {

	enum class STYLE { BORDER, FILLED, DASHED };

	class Surface
	{
	public:

		inline virtual void Draw() const {}
		inline virtual void OnScroll(float x, float y) {}
		inline virtual void OnMove(float x, float y) {}
		inline virtual float GetSurfaceArea() const { return 0; }

		//Loop through surface points
		virtual inline std::vector<IPoint>& GetSurfacePoints() { return _points; }
		inline void SetProjection(glm::mat4 proj) { _projection = proj; }
		std::string& GetType() { return _type; }
	protected:
		glm::mat4 _projection = glm::mat4(1.0);
		std::vector<IPoint> _points;
		bool _changed = false;
		std::string _type = "None";

	};

}