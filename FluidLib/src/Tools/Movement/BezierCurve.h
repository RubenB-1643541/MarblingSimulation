#pragma once

#include "../Movement.h"
#include "Util/Point.h"
#include "GL/glew.h"
#include <vector>

namespace FluidLib {

	class BezierCurve : public Movement
	{
	public:

		BezierCurve();
		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint: Stays on the line
		*/
		virtual FPoint Get(float x, float y) override;

		inline FPoint* GetPoints() { return _controlpoints; }
	private:
		FPoint _controlpoints[4];

		float _precision = 0.01f;
		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/movements/beziercurve_vertexshader.glsl";
		const char* fragment = "res/shaders/movements/beziercurve_fragmentshader.glsl";
	};

}