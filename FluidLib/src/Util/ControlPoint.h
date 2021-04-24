#pragma once

#include "Point.h"
#include "GL/glew.h"
#include <vector>
#include "Rendering.h"
#include "glm/gtx/transform.hpp"

namespace FluidLib {

	class ControlPoint : public FPoint
	{
	public:
		ControlPoint();
		ControlPoint(float x, float y);
		ControlPoint(const FPoint& p);

		void Draw();

		bool OnClick(float x, float y);
		void OnRelease();
		void OnMove(float x, float y);
		inline bool Selected() { return _selected; }
		inline void SetProjection(glm::mat4 proj) { _projection = proj; }
		void VisualTranslate(const FPoint& p);
		float GetTransX();
		float GetTransY();
	private:
		void Init();
		FPoint _trans = {0.0f,0.0f};
		bool _selected = false;
		float _size = 10.0f;

		glm::mat4 _projection = glm::mat4(1.0);

		static GLuint _buffer;
		static GLuint _shader;
		const char* vertex = "res/shaders/util/controlpoint_vertexshader.glsl";
		const char* fragment = "res/shaders/util/controlpoint_fragmentshader.glsl";
	};

}