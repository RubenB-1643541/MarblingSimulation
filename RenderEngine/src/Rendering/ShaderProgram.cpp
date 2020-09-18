#include "pch.h"
#include "ShaderProgram.h"

RenderEngine::ShaderProgram::ShaderProgram()
{
}

RenderEngine::ShaderProgram::ShaderProgram(Shader& shader)
{
	Create(shader);
}

/*
//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
*/
bool RenderEngine::ShaderProgram::Create(Shader& shader)
{
	_shader = shader;
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(shader.vertex, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		ENGINE_ERROR("Failed to open {}", shader.vertex);
		return false;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(shader.fragment, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		ENGINE_ERROR("Failed to open {}", shader.fragment);
		return false;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	//printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cerr << VertexShaderErrorMessage[0] << std::endl;
		//printf("%s\n", &VertexShaderErrorMessage[0]);
		if (!Result) {
			ENGINE_ERROR("Failed to Compile VertexShader {}", &VertexShaderErrorMessage[0]);
			return false;
		}
		else {
			ENGINE_ERROR("VertexShader info: {}", &VertexShaderErrorMessage[0]);
		}
	}

	// Compile Fragment Shader
	//printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		//printf("%s\n", &FragmentShaderErrorMessage[0]);
		if (!Result) {
			ENGINE_ERROR("Failed to Compile FragmentShader {}", &FragmentShaderErrorMessage[0]);
			return false;
		}
		else {
			ENGINE_ERROR("FragmentShader info {}", &FragmentShaderErrorMessage[0]);
		}
	}

	// Link the program
	//printf("Linking program\n");
	_id = glCreateProgram();
	glAttachShader(_id, VertexShaderID);
	glAttachShader(_id, FragmentShaderID);
	glLinkProgram(_id);

	// Check the program
	glGetProgramiv(_id, GL_LINK_STATUS, &Result);
	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		if (!Result) {
			ENGINE_ERROR("Failed to link VertexShader and FragmentShader {}", ProgramErrorMessage[0]);
			return false;
		}
		else {
			ENGINE_ERROR("VertexShader and FragmentShader Linking Message: {}", ProgramErrorMessage[0]);
		}
	}

	glDetachShader(_id, VertexShaderID);
	glDetachShader(_id, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return true;
}

void RenderEngine::ShaderProgram::Bind()
{
	glUseProgram(_id);
}

void RenderEngine::ShaderProgram::UnBind()
{
	glUseProgram(NULL);
}

void RenderEngine::ShaderProgram::UseProgram()
{
	glUseProgram(_id);
}
