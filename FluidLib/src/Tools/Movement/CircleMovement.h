#pragma once

#include "../Parameters.h"
#include "../Movement.h"
#include "Util/Point.h"
#include "GL/glew.h"
#include "Util/ControlPoint.h"
#include <vector>
#include <cmath>
#include "corecrt_math_defines.h"
#define _USE_MATH_DEFINES

namespace FluidLib {

	class CircleMovement : public Movement
	{
	public:
		CircleMovement();
		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		void StartEdit() override;
		void OnEditDraw() override;
		bool OnEditClick(float x, float y) override;
		bool OnEditRelease(float x, float y) override;
		bool OnEditMove(float x, float y) override;

		float GetR() { return _r; }
		void SetR(float r) { _r = r; }
		float* GetRPtr() { return &_r; }
		float GetX() { return _x;}
		void SetX(float x) { _x = x;}
		float* GetXPtr() { return &_x; }
		float GetY() { return _y;}
		void SetY(float y) { _y = y;}
		float* GetYPtr() { return &_y; }

		virtual FPoint Get(float x, float y) override;
		virtual float GetNormalRotation(float x, float y);
		void SetProjection(glm::mat4 proj) override;

		CircleMoveParameters GetParam();
		void LoadParam(Parameters* p) override;
	private:
		float _r = 50.0f;
		float _rold = 50.0f;
		float _x = 450.0f;
		float _y = 250.0f;
		float _color[3] = { 1.0,1.0,1.0 };

		ControlPoint _pos;
		ControlPoint _size;

		static GLuint _buffer;
		static GLuint _shader;
		const float _precision = 0.01f;
		const char* vertex = "res/shaders/surfaces/circle_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/circle_fragmentshader.glsl";

	};

}