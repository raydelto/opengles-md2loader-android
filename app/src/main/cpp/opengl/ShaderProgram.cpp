#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

#include "Logger.h"

using namespace Raydelto::MD2Loader;
using std::string;

ShaderProgram::ShaderProgram()
	: mHandle(0)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mHandle);
}

bool ShaderProgram::LoadShaders(std::string vsFilename, std::string fsFilename)
{

	string vsString = FileToString(vsFilename.c_str());
	string fsString = FileToString(fsFilename.c_str());
	const GLchar *vsSourcePtr = vsString.c_str();
	const GLchar *fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, nullptr);
	glShaderSource(fs, 1, &fsSourcePtr, nullptr);

	glCompileShader(vs);
	CheckCompileErrors(vs, ShaderType::VERTEX);

	glCompileShader(fs);
	CheckCompileErrors(fs, ShaderType::FRAGMENT);

	mHandle = glCreateProgram();
	if (mHandle == 0)
	{
		LOGE("Unable to create shader program!");
		return false;
	}

	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	glLinkProgram(mHandle);
	CheckCompileErrors(mHandle, ShaderType::PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();
	return true;
}

string ShaderProgram::FileToString(const string &filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);

		if (!file.fail())
		{
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception &ex)
	{
		LOGE("Error reading shader filename!");
	}

	return ss.str();
}

void ShaderProgram::Use() const
{
	if (mHandle > 0)
	{
		glUseProgram(mHandle);
	}
}

void ShaderProgram::CheckCompileErrors(GLuint shader, ShaderType type)
{
	int status = 0;

	if (type == ShaderType::PROGRAM)
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);

			// The length includes the NULL character
			string errorLog(length, ' '); // Resize and fill with space character
			glGetProgramInfoLog(shader, length, &length, &errorLog[0]);
			LOGE("Error! Shader program failed to link. %s length %d\n", errorLog.c_str(), length);
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

			// The length includes the NULL character
			string errorLog(length, ' '); // Resize and fill with space character
			glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
			LOGE("Error! Shader failed to compile. %s length %d\n", errorLog.c_str(), length);
		}
	}
}

GLuint ShaderProgram::GetProgram() const
{
	return mHandle;
}

void ShaderProgram::SetUniform(const GLchar *name, const float &f)
{
	GLint loc = GetUniformLocation(name);
	glUniform1f(loc, f);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec2 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec3 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::vec4 &v)
{
	GLint loc = GetUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::SetUniform(const GLchar *name, const glm::mat4 &m)
{
	GLint loc = GetUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

GLint ShaderProgram::GetUniformLocation(const GLchar *name)
{
	auto it = mUniformLocations.find(name);

	if (it == mUniformLocations.end())
	{
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	return mUniformLocations[name];
}
