#pragma once

#include "Tools/Surface.h"
#include "GL/glew.h"
#include <cmath>
#include "corecrt_math_defines.h"
#define _USE_MATH_DEFINES
namespace FluidLib {

	class Circle : public Surface
	{
	public:
		Circle();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;

	private:
		float _r = 50.0f;
		float _x = 0.0f;
		float _y = 0.0f;
		float _color[3] = { 1.0,1.0,1.0 };
		STYLE _style = STYLE::DASHED;
		static GLuint _buffer;
		static GLuint _shader;
		const float _precision = 0.01f;
		const char* vertex = "res/shaders/surfaces/circle_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/circle_fragmentshader.glsl";

	};

}