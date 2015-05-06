/*
===============================================================================

	XOF
	===
	File	:	XOF_Sprite.cpp
	Desc	:	Represents a basic 2D sprite with support for animation
				via sprite-sheet.

===============================================================================
*/
#include "XOF_Sprite.hpp"


Sprite::Sprite( U16 frameCount, U16 startingFrame, U16 subAnimationFrameCount ) {
	mAnimationFrameCount = frameCount;
	mStartingAnimationFrame = startingFrame;
	mCurrentAnimationFrame = static_cast<float>( startingFrame );
	mSubAnimationFrameCount = subAnimationFrameCount;

	GenerateVertexData();
}

Sprite::~Sprite() {
}

void Sprite::SetAnimationFrames( U16 startingFrame, U16 frameCount ) {
	mStartingAnimationFrame = startingFrame;
	mCurrentAnimationFrame = static_cast<float>( startingFrame );
	mSubAnimationFrameCount = frameCount;
	UpdateTextureCoordinates();
}

U16 Sprite::GetStartingFrame() const {
	return mStartingAnimationFrame;
}

void Sprite::StepAnimation( float animationDelta ) {
	mCurrentAnimationFrame += animationDelta;

	if( mCurrentAnimationFrame > ( mStartingAnimationFrame + mSubAnimationFrameCount ) ) {
		mCurrentAnimationFrame = mStartingAnimationFrame;
	}

	UpdateTextureCoordinates();
}

void Sprite::GenerateVertexData() {
	float startTexCoordX = (float)mStartingAnimationFrame / (float)mAnimationFrameCount;
	float endTexCoordX = startTexCoordX + 1.f / (float)mAnimationFrameCount;

	// 1x1 poly
	mVertices[0] = Vertex( glm::vec3( -0.5f,					0,						0.f ), glm::vec2( startTexCoordX,	0.f ) );
	mVertices[1] = Vertex( glm::vec3( mVertices[0].pos.x + 1,	0,						0.f ), glm::vec2( endTexCoordX,		0.f ) );
	mVertices[2] = Vertex( glm::vec3( mVertices[0].pos.x + 1,	mVertices[0].pos.y - 1,	0.f ), glm::vec2( endTexCoordX,		1.f ) );
	mVertices[3] = Vertex( glm::vec3( -0.5f,					mVertices[0].pos.y - 1,	0.f ), glm::vec2( startTexCoordX,	1.f ) );

	U32 indices[6] = { 0, 1, 2, 0, 2, 3 };

	// Don't bind and buffer yet since the data will be changing often
	glGenBuffers( 1, &mVB );

	glGenBuffers( 1, &mIB );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mIB );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( U32 ) * 6, &indices, GL_STATIC_DRAW );
}

// Has the effect of moving the sprite to a different frame in it's animation
void Sprite::UpdateTextureCoordinates() {
	float startTexCoordX = (int)mCurrentAnimationFrame / (float)mAnimationFrameCount;
	float endTexCoordX = startTexCoordX + 1.f / (float)mAnimationFrameCount;

	mVertices[0].texCoords.x = startTexCoordX;
	mVertices[1].texCoords.x = endTexCoordX;
	mVertices[2].texCoords.x = endTexCoordX;
	mVertices[3].texCoords.x = startTexCoordX;
}