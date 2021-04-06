#pragma once

#include "../Surface.h"
#include "GL/glew.h"
#include "Util/ControlPoint.h"
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

		virtual void EditDraw() override;
		virtual void StartEdit() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		std::vector<IPoint>& GetSurfacePoints() override;

		inline float GetAngle() { return _angle; }
		inline void SetAngle(float angle) { _angle = angle; }
		inline float* GetAnglePtr() { return &_angle; }

		inline float GetLen() { return _len; }
		inline void SetLen(float len) { _len = len; }
		inline float* GetLenPtr() { return &_len; }

		inline virtual void SetRotation(float rot) { _rotation = rot; }
		void SetProjection(glm::mat4 proj) override;
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _angle = 0.5;
		float _len = 50;

		ControlPoint _control;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/fan_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/fan_fragmentshader.glsl";
	};

}