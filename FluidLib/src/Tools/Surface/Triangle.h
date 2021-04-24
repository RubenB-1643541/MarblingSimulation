#pragma once

#include "../Parameters.h"
#include "Tools/Surface.h"
#include "GL/glew.h"
#include "Util/ControlPoint.h"

namespace FluidLib {

	class Triangle : public Surface
	{
	public:
		Triangle();
		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;
		inline void SetStyle(STYLE style) { _style = style; }
		inline void SetCentered(bool cent) { _centered = cent; }

		float GetWidth() {return _width;}
		float GetHeight() {return _height;}
		void SetWidth(float width){ _width = width;}
		void SetHeight(float height){ _height = height;}
		float* GetWidthPtr() {return &_width;}
		float* GetHeightPtr() {return &_height;}

		virtual void StartEdit() override;
		virtual void EditDraw() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		void SetProjection(glm::mat4 proj) override;

		TriangleSurfParameters GetParam();
		void LoadParam(Parameters* p) override;
	private:
		float _xpos = 0;
		float _ypos = 0;
		float _width = 50.0f;
		float _widthold;
		float _height = 60.0f;
		float _heightold;
		float _color[3] = { 1.0,1.0,1.0 };
		STYLE _style = STYLE::BORDER;
		bool _centered = true;

		ControlPoint _pos;
		ControlPoint _wcon;
		ControlPoint _hcon;
		ControlPoint _rotcon;

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/rect_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/rect_fragmentshader.glsl";
	};

}