/*
===============================================================================

	XOF
	===
	File	:	XOF_Texture.cpp
	Desc	:	Represents a texture map; diffuse, normal etc...

===============================================================================
*/
#include "XOF_Texture.hpp"
#include "../ThirdParty/stb_image.h"
#include <assert.h>
#include <iostream>


Texture::Texture() {
	mIsLoaded = false;
	mTexture = 0;
	mRawImageData = nullptr;
	mWidth = mHeight = 0;
}

Texture::Texture( const std::string &fileName, bool keepRawData ) {
	Load( fileName, keepRawData );
}

Texture::~Texture() {
	glDeleteTextures( 1, &mTexture );
	if( mRawImageData != nullptr ) {
		stbi_image_free( mRawImageData );
	}
}

bool Texture::Load( const std::string &fileName, bool keepRawData ) {
	I32 componentCount;
	mRawImageData = reinterpret_cast<U32*>( stbi_load( fileName.c_str(), 
		&mWidth, &mHeight, &componentCount, 4 ) );

	if( mRawImageData == nullptr ) {
		std::cerr << "Texture failed to load: " + fileName << std::endl;
		mIsLoaded = false;
		return mIsLoaded;
	}

	glGenTextures( 1, &mTexture );
	glBindTexture( GL_TEXTURE_2D, mTexture );

	// Set wrapping
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	// Set filtering
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Send texture to GPU
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, mRawImageData );

	if( !keepRawData ) {
		stbi_image_free( mRawImageData );
		mRawImageData = nullptr;
	}

	mIsLoaded = true;
	return mIsLoaded;
}

bool Texture::IsLoaded() const {
	return mIsLoaded;
}

I32	Texture::GetWidth() const {
	return mWidth;
}

I32	Texture::GetHeight() const {
	return mHeight;
}

U32 Texture::GetPixel( U32 x, U32 y ) const {
	return mRawImageData[x + y * mWidth];
}

void Texture::Bind( U32 unit ) {
	assert( ( unit >= 0 ) && ( unit <= 31 ) );
	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_2D, mTexture );
}