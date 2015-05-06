/*
===============================================================================

	XOF
	===
	File	:	XOF_Texture.hpp
	Desc	:	Represents a texture map; diffuse, normal etc...

===============================================================================
*/
#ifndef XOF_TEXTURE_HPP
#define XOF_TEXTURE_HPP


#include "../Platform/XOF_Platform.hpp"
#include <GL/glew.h>
#include <string>


enum XOF_TEXTURE_TYPE {
	DIFFUSE = 0,
	NORMAL,
	TEXTURE_TYPE_COUNT,
};


class Texture {
public:
			Texture();
			// Keep raw data will cause the texture to retain the raw bitmap data,
			// this allows for the accessing of individual pixels at the cost of extra memory.
			// Added for WolfX level construction.
			Texture( const std::string &fileName, bool keepRawData = false );
			~Texture();

	bool	Load( const std::string &fileName, bool keepRawData = false );
	bool	IsLoaded() const;

	I32		GetWidth() const;
	I32		GetHeight() const;
	U32		GetPixel( U32 x, U32 y ) const;

	void	Bind( U32 unit );

private:
	bool	mIsLoaded;
	GLuint	mTexture;
	I32		mWidth;
	I32		mHeight;
	U32*	mRawImageData;
};


using UniqueTexturePtr = std::unique_ptr<Texture>;


#endif // XOF_TEXTURE_HPP