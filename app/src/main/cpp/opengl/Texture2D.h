#pragma once
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
using std::string;

namespace Raydelto::MD2Loader
{
	class Texture2D
	{
	public:
		Texture2D();
		virtual ~Texture2D();

		bool loadTexture(const string &fileName, bool generateMipMaps = true);
		void bind(GLuint texUnit = 0);

	private:
		Texture2D(const Texture2D &rhs) = default;
		Texture2D &operator=(const Texture2D &rhs) = default;

		GLuint mTexture;
	};
}
