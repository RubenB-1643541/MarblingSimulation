#include "ShaderController.h"

namespace FluidLib {
	
	ShaderController::ShaderController()
	{
	}

	ShaderController::ShaderController(GLuint shader) : _shader(shader)
	{

	}
	
	void ShaderController::Use()
	{
		glUseProgram(_shader);
	}

	void ShaderController::SetShader(GLuint shader)
	{
		_shader = shader;
		_uniforms.SetShaderId(shader);
	}

}