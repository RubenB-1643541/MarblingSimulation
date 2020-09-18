#pragma once

#include "GL/glew.h"

namespace RenderEngine {

	class ComputeShaderProgram
	{
	public:
		bool Create(const char* compute_file_path);
		void Bind();
		void UnBind();

		void DispatchCompute(int groupsx, int groupsy, int groupsz);
		void DispatchComputeGroupSize(int groupsx, int groupsy, int groupsz, int groupsizex, int groupsizey, int groupsizez);
		void SetMemoryBarrier(GLbitfield barriers);

		void UseProgram();

		GLuint GetUniformLocation(std::string s);

		inline GLuint GetId() { return _id; }

	private:
		GLuint _id;

	};

}