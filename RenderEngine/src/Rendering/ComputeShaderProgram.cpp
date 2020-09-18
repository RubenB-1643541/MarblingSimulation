#include "pch.h"
#include "ComputeShaderProgram.h"

namespace RenderEngine {

	bool ComputeShaderProgram::Create(const char* compute_file_path)
	{
		// Create the shaders
		GLuint ComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);

		// Read the Compute Shader code from the file
		std::string ComputeShaderCode;
		std::ifstream ComputeShaderStream(compute_file_path, std::ios::in);
		if (ComputeShaderStream.is_open()) {
			std::stringstream sstr;
			sstr << ComputeShaderStream.rdbuf();
			ComputeShaderCode = sstr.str();
			ComputeShaderStream.close();
		}
		else {
			std::cerr << "Failed to open " << compute_file_path << std::endl;
			return false;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Compute Shader
		char const* VertexSourcePointer = ComputeShaderCode.c_str();
		glShaderSource(ComputeShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(ComputeShaderID);

		// Check Compute Shader
		glGetShaderiv(ComputeShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(ComputeShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ComputeShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(ComputeShaderID, InfoLogLength, NULL, &ComputeShaderErrorMessage[0]);
			std::cerr << ComputeShaderErrorMessage[0] << std::endl;
			//printf("%s\n", &VertexShaderErrorMessage[0]);
			if (!Result) {
				std::cerr << "Failed to compile ComputeShader: " << &ComputeShaderErrorMessage[0] << std::endl;
				return false;
			}
			else {
				std::cout << "ComputeShader info: " << &ComputeShaderErrorMessage[0] << std::endl;
			}
		}

		// Link the program
		//printf("Linking program\n");
		_id = glCreateProgram();
		glAttachShader(_id, ComputeShaderID);
		glLinkProgram(_id);

		// Check the program
		glGetProgramiv(_id, GL_LINK_STATUS, &Result);
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			if (!Result) {
				std::cerr << "Failed to link ComputeShader: " << &ProgramErrorMessage[0] << std::endl;
				return false;
			}
			else {
				std::cout << "ComputeShader Linking Message: " << &ProgramErrorMessage[0] << std::endl;
			}
		}

		glDetachShader(_id, ComputeShaderID);
		glDeleteShader(ComputeShaderID);

		return true;
	}

	void ComputeShaderProgram::Bind()
	{
		glUseProgram(_id);
	}

	void ComputeShaderProgram::UnBind()
	{
		glUseProgram(NULL);
	}

	void ComputeShaderProgram::DispatchCompute(int groupsx, int groupsy, int groupsz)
	{
		glDispatchCompute(groupsx, groupsy, groupsz);
	}

	void ComputeShaderProgram::DispatchComputeGroupSize(int groupsx, int groupsy, int groupsz, int groupsizex, int groupsizey, int groupsizez)
	{
		glDispatchComputeGroupSizeARB(groupsx, groupsy, groupsz, groupsizex, groupsizey, groupsizez);
	}

	void ComputeShaderProgram::SetMemoryBarrier(GLbitfield barriers)
	{
		glMemoryBarrier(barriers);
	}

	void ComputeShaderProgram::UseProgram()
	{
		glUseProgram(_id);
	}

	GLuint ComputeShaderProgram::GetUniformLocation(std::string s)
	{
		return glGetUniformLocation(_id, s.c_str());
	}

}