#include "ComputeShaderController.h"

namespace FluidLib {

	void ComputeShaderController::SetShader(GLuint id)
	{
		_computeshader = id;
		_uniforms.SetShaderId(id);
	}

	void ComputeShaderController::Use()
	{
		glUseProgram(_computeshader);
	}

	void ComputeShaderController::BindUniforms()
	{
		_uniforms.Bind();
	}

	void ComputeShaderController::Dispatch()
	{
		glDispatchCompute(_groupx, _groupy, _groupz);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}

	void ComputeShaderController::SetGroupSizes(int x, int y, int z)
	{
		_groupx = x; _groupy = y; _groupz = z;
	}
}