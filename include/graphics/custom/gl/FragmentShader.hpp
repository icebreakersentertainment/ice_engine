#ifndef FRAGMENTSHADER_H_
#define FRAGMENTSHADER_H_

#include "Shader.hpp"

namespace hercules
{
namespace graphics
{
namespace custom
{
namespace gl
{

class FragmentShader : public Shader<FragmentShader>
{
public:
	using Shader<FragmentShader>::Shader;
	/*
	FragmentShader() : Shader<FragmentShader>::Shader()
	{
	}

	explicit FragmentShader(const GLuint id) : Shader<FragmentShader>::Shader(id)
	{
	}
	
	explicit FragmentShader(const std::string& source)
	{
		Shader<FragmentShader>::Shader(source);
	}
	
	FragmentShader(const FragmentShader& other) = delete;
	
	FragmentShader(FragmentShader&& other) : Shader<FragmentShader>::Shader(std::move(other))
	{
	}
	
	~FragmentShader() = default;
	
	const FragmentShader& operator=(const FragmentShader& other) = delete;
	*/
	/*
	const FragmentShader& operator=(const FragmentShader& other)
	{
		
	}
	*/

	GLenum type() const
	{
		return GL_FRAGMENT_SHADER;
	}
};

}
}
}
}

#endif /* FRAGMENTSHADER_H_ */

