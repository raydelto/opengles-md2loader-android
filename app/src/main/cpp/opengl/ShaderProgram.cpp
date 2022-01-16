#include "ShaderProgram.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "glm/gtc/type_ptr.hpp"

#include "Logger.h"

using namespace Raydelto::MD2Loader;

ShaderProgram::ShaderProgram()
	: mHandle(0)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(mHandle);
}

bool ShaderProgram::loadShaders(const char *vsFilename, const char *fsFilename)
{

	string vsString = fileToString(vsFilename);
	string fsString = fileToString(fsFilename);
	const GLchar *vsSourcePtr = vsString.c_str();
	const GLchar *fsSourcePtr = fsString.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vs, 1, &vsSourcePtr, nullptr);
	glShaderSource(fs, 1, &fsSourcePtr, nullptr);

	glCompileShader(vs);
	checkCompileErrors(vs, ShaderType::VERTEX);

	glCompileShader(fs);
	checkCompileErrors(fs, ShaderType::FRAGMENT);

	mHandle = glCreateProgram();
	if (mHandle == 0)
	{
		LOGE("Unable to create shader program!");
		return false;
	}

	glAttachShader(mHandle, vs);
	glAttachShader(mHandle, fs);

	glLinkProgram(mHandle);
	checkCompileErrors(mHandle, ShaderType::PROGRAM);

	glDeleteShader(vs);
	glDeleteShader(fs);

	mUniformLocations.clear();
	return true;
}

string ShaderProgram::fileToString(const string &filename)
{
	std::stringstream ss;
	std::ifstream file;

	try
	{
		file.open(filename, std::ios::in);

		if (!file.fail())
		{
			// Using a std::stringstream is easier than looping through each line of the file
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

void ShaderProgram::use() const
{
	if (mHandle > 0)
		glUseProgram(mHandle);
}

void ShaderProgram::checkCompileErrors(GLuint shader, ShaderType type)
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

GLuint ShaderProgram::getProgram() const
{
	return mHandle;
}

void ShaderProgram::setUniform(const GLchar *name, const float &f)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, f);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec2 &v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec3 &v)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::vec4 &v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const GLchar *name, const glm::mat4 &m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

GLint ShaderProgram::getUniformLocation(const GLchar *name)
{
	auto it = mUniformLocations.find(name);

	// Only need to query the shader program IF it doesn't already exist.
	if (it == mUniformLocations.end())
	{
		// Find it and add it to the map
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	// Return it
	return mUniformLocations[name];
}
