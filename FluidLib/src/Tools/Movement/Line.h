#pragma once

#include "../Movement.h"
#include "Util/Point.h"
#include "GL/glew.h"
#include <vector>

namespace FluidLib {

	class Line : public Movement
	{
	public:

		Line();
		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint: Stays on the line
		*/
		virtual FPoint Get(float x, float y) override;
		float* GetX() { return &_xpos; }
		float* GetY() { return &_ypos; }
		float* GetLen() { return &_len; }
		
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _len = 0;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/movements/line_vertexshader.glsl";
		const char* fragment = "res/shaders/movements/line_fragmentshader.glsl";
	};

}