#pragma once

#include "ShaderController.h"
#include "../GridManager.h"
#include "glm/gtx/transform.hpp"

namespace FluidLib {

	class GridRenderer
	{
	public:
		GridRenderer();
		GridRenderer(GridManager* grids);

		void Draw();
		inline void SetShader(GLuint shader) { _shader.SetShader(shader); }
		inline void SetSize(int size) { _size = size; }
		void SetScreenSize(float width, float height);
		inline ShaderController* GetShader() { return &_shader; }
	private:
		ShaderController _shader;
		GridManager * _grids = nullptr;
		int _size = 1;

		glm::mat4 _projection;

	};

}