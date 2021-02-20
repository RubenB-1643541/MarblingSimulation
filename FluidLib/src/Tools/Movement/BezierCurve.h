#pragma once

#include "../Movement.h"
#include "Util/ControlPoint.h"
#include "GL/glew.h"
#include <vector>

namespace FluidLib {

	class BezierCurve : public Movement
	{
	public:

		BezierCurve();
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
		virtual float GetNormalRotation(float x, float y) override;

		inline ControlPoint* GetPoints() { return _controlpoints; }
		void SetProjection(glm::mat4 proj) override;
	private:
		ControlPoint _controlpoints[4];

		float _precision = 0.01f;
		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/movements/beziercurve_vertexshader.glsl";
		const char* fragment = "res/shaders/movements/beziercurve_fragmentshader.glsl";
	};

}