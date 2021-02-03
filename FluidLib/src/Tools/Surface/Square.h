#pragma once

#include "Tools/Surface.h"
#include "GL/glew.h"
#include <Util/ControlPoint.h>

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

		virtual void StartEdit() override;
		virtual void EditDraw() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		void SetProjection(glm::mat4 proj) override;
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _len = 50.0f;
		float _color[3] = { 1.0,1.0,1.0 };
		STYLE _style = STYLE::BORDER;
		bool _centered = false;

		ControlPoint _pos;
		ControlPoint _size;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/rect_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/rect_fragmentshader.glsl";
	};

}