#pragma once

#include <string>
#include <gl/glew.h>
#include "Core/Log.h"

namespace RenderEngine {

	struct Shader {
		std::string vertex = "";
		std::string fragment = "";
		std::string geometry = "";
	};

	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(Shader& shader);
		//TODO Add Geometry Shader Support
		bool Create(Shader& shader);
		void Bind();
		void UnBind();

		void UseProgram();

		inline GLuint GetId() { return _id; }
		GLuint GetUniformLocation(std::string s);
	private:
		GLuint _id = -1;
		Shader _shader;

		
	};

}