#include "Rendering.h"

GLuint CompileShader(const char* vertex, const char* fragment)
{

	GLuint id = -1;
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		std::cerr << "Failed to open " << vertex << std::endl;
		return id;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		std::cerr << "Failed to open " << fragment << std::endl;
		return id;
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
			std::cerr << "Failed to Compile VertexShader " << &VertexShaderErrorMessage[0] << std::endl;
			return id;
		}
		else {
			std::cerr << "VertexShader info: " << &VertexShaderErrorMessage[0] << std::endl;
		}
	}

	// Compile Fragment Shader
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
			std::cerr << "Failed to Compile FragmentShader " << &FragmentShaderErrorMessage[0] << std::endl;
			return id;
		}
		else {
			std::cout << "FragmentShader info: " << &FragmentShaderErrorMessage[0] << std::endl;
		}
	}

	// Link the program
	//printf("Linking program\n");
	id = glCreateProgram();
	glAttachShader(id, VertexShaderID);
	glAttachShader(id, FragmentShaderID);
	glLinkProgram(id);

	// Check the program
	glGetProgramiv(id, GL_LINK_STATUS, &Result);
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		if (!Result) {
			std::cerr << "Failed to link VertexShader and FragmentShader {}" << &ProgramErrorMessage[0] << std::endl;
			return -1;
		}
		else {
			std::cout << "VertexShader and FragmentShader Linking Message: {}" << &ProgramErrorMessage[0] << std::endl;
		}
	}

	glDetachShader(id, VertexShaderID);
	glDetachShader(id, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return id;
}
