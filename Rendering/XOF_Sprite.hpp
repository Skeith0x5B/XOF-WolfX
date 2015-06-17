/*
===============================================================================

	XOF
	===
	File	:	XOF_Sprite.hpp
	Desc	:	Represents a basic 2D sprite with support for animation
				via sprite-sheet.

===============================================================================
*/
#ifndef XOF_SPRITE_HPP
#define XOF_SPRITE_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Vertex.hpp"
#include <GL/glew.h>
class SpriteManager;


class Sprite {
public:
	friend class	SpriteManager;

					Sprite( U8 frameCount = 1, U8 startingFrame = 0, U8 subAnimationFrameCount = 1 );
					~Sprite();

	void			SetAnimationFrames( U8 startingFrame, U8 frameCount );
	void			ResetAnimation();
	U8				GetStartingFrame() const;
	U8				GetCurrentFrame() const;
	U8				GetAnimationFrameCount() const;

	void			StepAnimation( float animationDelta );

	void			GenerateVertexData();
	UINT&			GetVertexBuffer() const;
	UINT&			GetIndexBuffer() const;

//private:
	Vertex			mVertices[4];

	GLuint			mVB;
	GLuint			mIB;

	float			mCurrentAnimationFrame;
	U8				mAnimationFrameCount;
	U8				mSubAnimationFrameCount;
	U8				mStartingAnimationFrame;
	U8				PADDING; // Just padding for alignment/quick access, can be removed

	void			UpdateTextureCoordinates();
};


using UniqueSpritePtr = std::unique_ptr<Sprite>;


#endif // XOF_SPRITE_HPP