#pragma once

#include "../Surface.h"
#include "GL/glew.h"

#include <cmath>
#include <corecrt_math_defines.h>
#define _USE_MATH_DEFINES

namespace FluidLib {

	class FanSurface : public Surface
	{
	public:
		FanSurface();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;

		std::vector<IPoint>& GetSurfacePoints() override;

		inline float GetAngle() { return _angle; }
		inline void SetAngle(float angle) { _angle = angle; }
		inline float* GetAnglePtr() { return &_angle; }

		inline float GetLen() { return _len; }
		inline void SetLen(float len) { _len = len; }
		inline float* GetLenPtr() { return &_len; }
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _angle = 0.5;
		float _len = 50;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/fan_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/fan_fragmentshader.glsl";
	};

}