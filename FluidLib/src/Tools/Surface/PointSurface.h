#pragma once

#include "../Surface.h"
#include "GL/glew.h"

namespace FluidLib {

	class PointSurface : public Surface
	{
	public:

		PointSurface();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;
	private:
		float _xpos = 0.0f;
		float _ypos = 0.0f;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/util/controlpoint_vertexshader.glsl";
		const char* fragment = "res/shaders/util/controlpoint_fragmentshader.glsl";
	};

}