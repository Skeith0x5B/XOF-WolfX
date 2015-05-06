/*
===============================================================================

	XOF
	===
	File	:	XOF_Shader.hpp
	Desc	:	Represents a shader; vertex, pixel (fragment) etc...

===============================================================================
*/
#ifndef XOF_SHADER_HPP
#define XOF_SHADER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Transform.hpp"
#include <GL/glew.h>
#include <unordered_map>


enum XOF_SHADER_TYPE {
	VERTEX = 0,
	FRAGMENT,
	// TESS_CONTROL, TESS_EVAL, GEOMETRY...
	SHADER_TYPE_COUNT,
};


class Shader {
public:
			Shader();
			Shader( const std::string &fileName );
			~Shader();

	bool	Load( const std::string & fileName );
	bool	IsLoaded() const;

	void	AddUniform( const std::string &uniformName );
	void	SetUniform( const std::string &uniformName, float value );
	void	SetUniform( const std::string &uniformName, glm::vec3 &value );
	void	SetUniform( const std::string &uniformName, glm::mat4 &value );

	void	Bind();
	
private:
	bool	mIsLoaded;
	
	GLuint	mProgram;
	GLuint	mShaders[XOF_SHADER_TYPE::SHADER_TYPE_COUNT];
	std::unordered_map<std::string, U32> mUniforms;
};


using UniqueShaderPtr = std::shared_ptr<Shader>;


#endif // XOF_SHADER_HPP