#pragma once

#include "GL/glew.h"
#include <string>


struct Icon {
    int width = 0;
    int height = 0;
	GLuint id = -1;
};

Icon Load(const std::string& name);