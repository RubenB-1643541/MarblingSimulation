#pragma once
#include "Tools/Surface.h"
#include "GL/glew.h"
#include <Util/ControlPoint.h>

namespace FluidLib {

	class Rectangle : public Surface
	{
	public:
		Rectangle();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;

		inline float* GetXPtr() { return &_xpos; }
		inline float* GetYPtr() { return &_ypos; }
		inline float GetX() { return _xpos; }
		inline float GetY() { return _ypos; }
		inline void SetX(float x) { _xpos = x; _changed = true; }
		inline void SetY(float y) { _ypos = y; _changed = true; }

		inline float* GetWidthPtr() { return &_width; }
		inline float* GetHeightPtr() { return &_height; }
		inline float GetWidth() { return _width; }
		inline float GetHeight() { return _height; }
		inline void SetWidth(float width) { _width = width; _changed = true; }
		inline void SetHeight(float height) { _height = height; _changed = true; }

		inline void SetStyle(STYLE style) { _style = style; }
		inline void SetCentred(bool cen) { _centered = cen; _changed = true; }

		virtual void StartEdit() override;
		virtual void EditDraw() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		void SetProjection(glm::mat4 proj) override;
	private:
		//ControlPoint _controlpoints[4];

		ControlPoint _pos;
		ControlPoint _size;
		ControlPoint _rotcon;
		float _xpos = 0;
		float _ypos = 0;
		float _width = 75.0f;
		float _widthold;
		float _height = 50.0f;
		float _heightold;
		float _color[3] = { 1.0,1.0,1.0 };
		STYLE _style = STYLE::BORDER;
		bool _centered = false;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/rect_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/rect_fragmentshader.glsl";
	};

}

