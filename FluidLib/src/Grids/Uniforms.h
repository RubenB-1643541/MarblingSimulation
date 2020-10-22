#pragma once

#include <vector>
#include <string>
#include "GL/glew.h"

namespace FluidLib {

	enum class UniformType {
		INT, FLOAT, INT_PTR, FLOAT_PTR
	};

	union UniformVal {
		int intval;
		float floatval;
		int* intptr;
		float* floatptr;
	};

	struct Uniform {
		UniformType type;
		UniformVal val;
		std::string strloc;
		int loc = -1;
		Uniform(UniformType t, UniformVal v, std::string s) { type = t; val = v; strloc = s; }
	};

	class Uniforms
	{
	public:
		Uniforms();
		Uniforms(GLuint shaderid);
		void SetShaderId(GLuint shaderid);
		void Bind();
		inline void Add(Uniform uniform) { _uniforms.push_back(uniform); }
	private:
		std::vector<Uniform> _uniforms;
		GLuint _shaderid = -1;
	};

}