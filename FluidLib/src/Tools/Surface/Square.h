#pragma once

#include "Tools/Surface.h"
#include "GL/glew.h"

namespace FluidLib {

	class Square : public Surface
	{
	public:
		Square();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;
		inline float* GetLen() { return &_len; }
		inline void SetStyle(STYLE style) { _style = style; }
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _len = 1.0f;
		float _color[3] = { 1.0,1.0,1.0 };
		STYLE _style = STYLE::BORDER;
		bool _centered = false;

	

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/rect_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/rect_fragmentshader.glsl";
	};

}