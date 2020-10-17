#pragma once

#include "../Movement.h"
#include "Util/Point.h"
#include "GL/glew.h"
#include <vector>
#include "corecrt_math_defines.h"
#define _USE_MATH_DEFINES

namespace FluidLib {

	class Sine : public Movement
	{
	public:
		Sine();

		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		/*
		Changes input position to new position
		@param float x, y : mouse position
		@return FPoint: Stays on the sinus line
		*/
		virtual FPoint Get(float x, float y) override;
		float* GetVerttrans() { return &_verttrans; }
		float* GetHortrans() { return &_hortrans; }
		float* GetAmp() { return &_amplitude; }
		float* GetPeriod() { return &_period; }

	private:
		float _verttrans = 250.0f;
		float _hortrans = 0.0f;
		float _amplitude = 30.0f;
		float _period = 30.0f;


		std::vector<float> _sinepoints;
		const float _precision = 0.001f;
		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/movements/sine_vertexshader.glsl";
		const char* fragment = "res/shaders/movements/sine_fragmentshader.glsl";

	};

}