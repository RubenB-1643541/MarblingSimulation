#pragma once

#include "GL/glew.h"

class ComputeShaderController
{
public:

	void SetShader(GLuint id);
	void Use();
	void BindUniforms();
	void Dispatch();
	inline bool IsShaderSet() { return _computeshader == -1; }
	void SetGroupSizes(int x = 1, int y = 1, int z = 1);
private:
	GLuint _computeshader = -1;
	int _groupx = 1, _groupy = 1, _groupz = 1;


};

