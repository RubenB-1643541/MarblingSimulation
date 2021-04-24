#pragma once

#include "../Parameters.h"
#include "../Movement.h"
#include "Util/ControlPoint.h"
#include <vector>

namespace FluidLib {

	class PointMovement : public Movement
	{
	public:
		PointMovement();
		bool OnScroll(float x, float y) override;
		void DrawPath() override;

		void StartEdit() override;
		void OnEditDraw() override;
		bool OnEditClick(float x, float y) override;
		bool OnEditRelease(float x, float y) override;
		bool OnEditMove(float x, float y) override;

		inline ControlPoint* GetPosPtr() { return &_pos; }
		inline FPoint GetPos() { return _pos; }

		virtual FPoint Get(float x, float y) override;
		void SetProjection(glm::mat4 proj) override;

		PointMoveParameters GetParam();
		void LoadParam(Parameters* p) override;
	private:
		ControlPoint _pos;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/util/controlpoint_vertexshader.glsl";
		const char* fragment = "res/shaders/util/controlpoint_fragmentshader.glsl";
	};

}