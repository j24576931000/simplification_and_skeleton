#include "ShaderObject.h"



ShaderObject::ShaderObject()
{
}


ShaderObject::~ShaderObject()
{
	for (std::list<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	if (program != 0)
	{
		glDeleteProgram(program);
		program = 0;
	}

}

bool ShaderObject::Init()
{
	program = glCreateProgram();
	if (program == 0)
	{
		puts("Create program error\n");
		return false;
	}
	else
		std::cout << "program ok" << std::endl;

	return true;
}

bool ShaderObject::AddShader(GLenum shaderType, std::string fileName)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		printf("Create shader error: %d\n", shaderType);
		return false;
	}
	else
		std::cout << fileName<<"shader ok" << std::endl;


	shaderList.push_back(shader);

	char **shaderSource = Common::LoadShaderSource((fileName).c_str());

	glShaderSource(shader, 1, shaderSource, NULL);
	Common::FreeShaderSource(shaderSource);

	glCompileShader(shader);
	bool isCompiled = Common::CheckShaderCompiled(shader);
	if (!isCompiled)
	{
		printf("Shader compile error %s \n", fileName.c_str());
		return false;
	}
	else
		std::cout << "shader compile ok" << std::endl;

	glAttachShader(program, shader);

	return Common::CheckGLError();
}

bool ShaderObject::Finalize()
{
	glLinkProgram(program);
	bool isLinked = Common::CheckProgramLinked(program);

	for (std::list<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	return isLinked;
}

void ShaderObject::Enable()
{
	glUseProgram(program);
}

void ShaderObject::Disable()
{
	glUseProgram(0);
}

GLuint ShaderObject::GetUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(program, name);
}