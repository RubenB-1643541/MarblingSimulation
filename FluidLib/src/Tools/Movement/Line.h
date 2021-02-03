#pragma once

#include "../Movement.h"
#include "Util/Point.h"
#include "GL/glew.h"
#include <vector>
#include <Util/ControlPoint.h>
#include <cmath>

namespace FluidLib {

	class Line : public Movement
	{
	public:

		Line();
		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		void StartEdit() override;
		void OnEditDraw() override;
		bool OnEditClick(float x, float y) override;
		bool OnEditRelease(float x, float y) override;
		bool OnEditMove(float x, float y) override;
		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint: Stays on the line
		*/
		virtual FPoint Get(float x, float y) override;

		FPoint* GetP1Ptr() { return &_p1; }
		FPoint* GetP2Ptr() { return &_p2; }

		void SetProjection(glm::mat4 proj) override;
		
	private:
		ControlPoint _p1;
		ControlPoint _p2;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/movements/line_vertexshader.glsl";
		const char* fragment = "res/shaders/movements/line_fragmentshader.glsl";
	};

}