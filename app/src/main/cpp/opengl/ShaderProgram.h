#pragma once

#include <string>
#include <unordered_map>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "glm/glm.hpp"

namespace Raydelto::MD2Loader
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool LoadShaders(const char *vsFilename, const char *fsFilename);
		void Use() const;

		void SetUniform(const GLchar *name, const float &f);
		void SetUniform(const GLchar *name, const glm::vec2 &v);
		void SetUniform(const GLchar *name, const glm::vec3 &v);
		void SetUniform(const GLchar *name, const glm::vec4 &v);
		void SetUniform(const GLchar *name, const glm::mat4 &m);

		GLint GetUniformLocation(const GLchar *name);
		GLuint GetProgram() const;

	private:
		enum class ShaderType
		{
			VERTEX,
			FRAGMENT,
			PROGRAM
		};

		std::string FileToString(const std::string &filename);
		void CheckCompileErrors(GLuint shader, ShaderType type);

		GLuint mHandle;
		std::unordered_map<std::string, GLint> mUniformLocations;
	};
}
