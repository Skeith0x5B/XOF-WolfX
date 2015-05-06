/*
===============================================================================

	XOF
	===
	File	:	XOF_SpriteManager.hpp
	Desc	:	Manage the sprites used by the game objects.

===============================================================================
*/
#ifndef XOF_SPRITE_MANAGER_HPP
#define XOF_SPRITE_MANAGER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_Sprite.hpp"
#include <unordered_map>


struct SpriteAnimationParams {
	U16 frameCount;
	U16 startingFrame;
	U16 subAnimationFrameCount;
};


class SpriteManager {
public:
	static SpriteManager*	Get();

	bool					StartUp();
	void					ShutDown();

	Sprite*					AddSprite( const std::string &spriteName, 
								const SpriteAnimationParams *animationParams = nullptr );
	Sprite*					GetSprite( const std::string &spriteName ) const;

private:
	// TODO: Replace string with hash value? Container already has
	std::unordered_map<std::string, Sprite>	mSprites;
};


#endif // XOF_SPRITE_MANAGER_HPP