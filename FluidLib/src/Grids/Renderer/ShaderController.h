#pragma once

#include <gl/glew.h>
#include "../Uniforms.h"

namespace FluidLib {

	class ShaderController
	{
	public:
		ShaderController();
		ShaderController(GLuint shader);

		
		void Use();
		inline bool IsShaderSet() { return _shader != -1; }
		void SetShader(GLuint shader);
		inline void BindUniforms() { _uniforms.Bind(); }
		inline GLuint GetId() { return _shader; }
		inline void AddUniform(Uniform uniform) { _uniforms.Add(uniform); }
	private:
		GLuint _shader = -1;
		Uniforms _uniforms;

	};

}