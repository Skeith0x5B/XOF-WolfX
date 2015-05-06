/*
===============================================================================

	XOF
	===
	File	:	XOF_Shader.cpp
	Desc	:	Represents a shader; vertex, pixel (fragment) etc...

===============================================================================
*/
#include "XOF_Shader.hpp"


// Utility code/helpers (move to a manager)
#include <fstream>
#include <iostream>


static std::string LoadShader(const std::string &fileName);
static bool CheckShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage );
static GLuint CreateShader( const std::string& shaderCode, GLenum shaderType );


// Shader class
Shader::Shader() {
	mIsLoaded = false;
}

Shader::Shader( const std::string &fileName ) {
	Load( fileName );
}

Shader::~Shader() {
	for( U8 i=0; i<XOF_SHADER_TYPE::SHADER_TYPE_COUNT; ++i ) {
		glDetachShader( mProgram, mShaders[i] );
		glDeleteShader( mShaders[i] );
	}
	glDeleteProgram( mProgram );
}

bool Shader::Load( const std::string & fileName ) {
	mProgram = glCreateProgram();
	mShaders[XOF_SHADER_TYPE::VERTEX] = CreateShader( LoadShader( fileName + ".glvs" ), GL_VERTEX_SHADER );
	mShaders[XOF_SHADER_TYPE::FRAGMENT] = CreateShader( LoadShader( fileName + ".glfs" ), GL_FRAGMENT_SHADER );

	// Moving this to stand-alone function similar to those for
	// uniforms causes issues like distorted textures
	// Similar to binding a vertexLayoutDesc in D3D
	glBindAttribLocation( mProgram, 0, "position" );
	glBindAttribLocation( mProgram, 1, "normal" );
	glBindAttribLocation( mProgram, 2, "texCoord" );
	//glBindAttribLocation( mProgram, 3, "color" );

	// Add shaders to overarching shader program
	for( U32 i=0; i<XOF_SHADER_TYPE::SHADER_TYPE_COUNT; ++i ) {
		glAttachShader( mProgram, mShaders[i] );
	}

	glLinkProgram( mProgram );
	mIsLoaded = CheckShaderError( mProgram, GL_LINK_STATUS, true, "Error: Shader program linking failed: " );

	glValidateProgram( mProgram );
	mIsLoaded = CheckShaderError( mProgram, GL_VALIDATE_STATUS, true, "Error: Shader program invalid: " );

	return mIsLoaded;
}

bool Shader::IsLoaded() const {
	return mIsLoaded;
}

void Shader::AddUniform( const std::string &uniformName ) {
	I32 uniformLocation = glGetUniformLocation( mProgram, uniformName.c_str() );

	if( uniformLocation == -1 ) {
		std::cerr << "Error: Uniform variable: " << uniformName << " not found in shader" << std::endl;
		return;
	}

	mUniforms[uniformName] = uniformLocation;
}

void Shader::SetUniform( const std::string &uniformName, float value ) {
	glUniform1f( mUniforms[uniformName], value );
}

void Shader::SetUniform( const std::string &uniformName, glm::vec3 &value ) {
	glUniform3fv( mUniforms[uniformName], 1, &value.x );
}

void Shader::SetUniform( const std::string &uniformName, glm::mat4 &value ) {
	glUniformMatrix4fv( mUniforms[uniformName], 1, GL_FALSE, &value[0][0] );
}

void Shader::Bind() {
	glUseProgram( mProgram );
}


// Utility code/helpers (move to a manager)
static std::string LoadShader( const std::string& fileName ) {
	std::ifstream file;
	file.open( fileName.c_str() );

	std::string output = "shader " + fileName + " not loaded";
	std::string line;

	if( file.is_open() ) {
		output.clear();
		while( file.good() ) {
			getline( file, line );
			output.append( line + "\n" );
		}
	} else {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}


static GLuint CreateShader( const std::string& shaderCode, GLenum shaderType ) {
	GLuint shader = glCreateShader( shaderType );

	if( shader == 0 ) {
		std::cerr << "Error: Shader creation failed!" << std::endl;
	}

	const GLchar* shaderSourceStrings[1];
	shaderSourceStrings[0] = shaderCode.c_str();

	GLint shaderSourceStringsLengths[1];
	shaderSourceStringsLengths[0] = shaderCode.length();

	// Finally, send the shader source code to OpenGL for compilation
	glShaderSource( shader, 1, shaderSourceStrings, shaderSourceStringsLengths );
	glCompileShader( shader );

	CheckShaderError( shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed: " );

	return shader;
}


static bool CheckShaderError( GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage ) {
	GLint param = 0;
	GLchar error[1024] = { 0 };

	if( isProgram ) {
		glGetProgramiv( shader, flag, &param );
	}
	else {
		glGetShaderiv( shader, flag, &param );
	}

	if( param == GL_FALSE ) {
		if( isProgram ) {
			glGetProgramInfoLog( shader, sizeof( error ), NULL, error );
		}
		else {
			glGetShaderInfoLog( shader, sizeof( error ), NULL, error );
		}
		std::cerr << errorMessage << ": " << error << std::endl;
		return false;
	}

	return true;
}