#pragma once

#include "Tools/Surface.h"
#include "GL/glew.h"
#include <Util/ControlPoint.h>

namespace FluidLib {


	class Polygon : public Surface
	{
	public:
		Polygon();

		void Draw() const override;
		void OnScroll(float x, float y) override;
		void OnMove(float x, float y) override;
		float GetSurfaceArea() const override;
		//Loop through surface points
		std::vector<IPoint>& GetSurfacePoints() override;

		inline float GetX() { return _pos.GetX(); }
		inline float GetY() { return _pos.GetY(); }
		inline void SetX(float x) { _pos.SetX(x); _changed = true; }
		inline void SetY(float y) { _pos.SetY(y); _changed = true; }

		virtual void StartEdit() override;
		virtual void EditDraw() override;
		virtual bool OnEditMove(float x, float y) override;
		virtual bool OnEditClick(float x, float y) override;
		virtual bool OnEditRelease(float x, float y) override;

		void AddPoint(const ControlPoint& point);
		void RemovePoint(int i);
		ControlPoint& GetPoint(int i);
		int PointCount() const { return _controlpoints.size(); }

		void SetProjection(glm::mat4 proj) override;
	private:
		void Refresh();
		std::vector<ControlPoint> _controlpoints;
		FPoint _pos;
		float _color[3] = { 1.0,1.0,1.0 };

		GLuint _buffer = -1;
		static GLuint _shader;
		const char* vertex = "res/shaders/surfaces/polygon_vertexshader.glsl";
		const char* fragment = "res/shaders/surfaces/polygon_fragmentshader.glsl";
	};

}