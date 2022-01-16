#pragma once

#include <string>
#include <map>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "glm/glm.hpp"
using std::string;

namespace Raydelto::MD2Loader
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool loadShaders(const char *vsFilename, const char *fsFilename);
		void use() const;

		GLuint getProgram() const;

		void setUniform(const GLchar *name, const float &f);
		void setUniform(const GLchar *name, const glm::vec2 &v);
		void setUniform(const GLchar *name, const glm::vec3 &v);
		void setUniform(const GLchar *name, const glm::vec4 &v);
		void setUniform(const GLchar *name, const glm::mat4 &m);

		GLint getUniformLocation(const GLchar *name);

	private:
		enum class ShaderType
		{
			VERTEX,
			FRAGMENT,
			PROGRAM
		};

		string fileToString(const string &filename);
		void checkCompileErrors(GLuint shader, ShaderType type);

		GLuint mHandle;
		std::map<string, GLint> mUniformLocations;
	};
}
