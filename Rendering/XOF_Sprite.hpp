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

					Sprite( U16 frameCount = 1, U16 startingFrame = 0, U16 subAnimationFrameCount = 1 );
					~Sprite();

	void			SetAnimationFrames( U16 startingFrame, U16 frameCount );
	U16				GetStartingFrame() const;
	void			StepAnimation( float animationDelta );

	void			GenerateVertexData();

//private: TEMP: CHANGE THIS BACK
	Vertex			mVertices[4];

	GLuint			mVB;
	GLuint			mIB;

	U16				mAnimationFrameCount;
	U16				mSubAnimationFrameCount;
	U16				mStartingAnimationFrame;
	float			mCurrentAnimationFrame;

	void			UpdateTextureCoordinates();
};


using UniqueSpritePtr = std::unique_ptr<Sprite>;


#endif // XOF_SPRITE_HPP