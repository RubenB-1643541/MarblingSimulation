#include "Uniforms.h"

namespace FluidLib {

	Uniforms::Uniforms()
	{
	}

	Uniforms::Uniforms(GLuint shaderid) : Uniforms()
	{
		SetShaderId(shaderid);
	}

	void Uniforms::SetShaderId(GLuint shaderid)
	{
		_shaderid = shaderid;
	}

	void Uniforms::Bind()
	{
		for (Uniform& u : _uniforms) {
			if (u.loc == -1)
				u.loc = glGetUniformLocation(_shaderid, u.strloc.c_str());
			switch (u.type)
			{
			case UniformType::INT:
				glUniform1i(u.loc, u.val.intval);
				break;
			case UniformType::FLOAT:
				glUniform1f(u.loc, u.val.floatval);
				break;
			default:
				break;
			}
		}
	}

}