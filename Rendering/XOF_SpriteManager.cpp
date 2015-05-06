/*
===============================================================================

	XOF
	===
	File	:	XOF_SpriteManager.cpp
	Desc	:	Manage the sprites used by the game objects.

===============================================================================
*/
#include "XOF_SpriteManager.hpp"


SpriteManager* SpriteManager::Get() {
	static SpriteManager singletonInstance;
	return &singletonInstance;
}

bool SpriteManager::StartUp() {
	return true;
}
	
void SpriteManager::ShutDown() {
	mSprites.clear();
}

Sprite* SpriteManager::AddSprite( const std::string &spriteName, const SpriteAnimationParams *animationParams ) {
	Sprite *spritePtr = GetSprite( spriteName );
	if( !spritePtr ) {
		// Setup a sprite
		Sprite sprite;
		if( animationParams != nullptr ) {
			sprite.mAnimationFrameCount = animationParams->frameCount;
			sprite.mStartingAnimationFrame = animationParams->startingFrame;
			sprite.mSubAnimationFrameCount = animationParams->subAnimationFrameCount;
			sprite.GenerateVertexData();
		}
		
		// Add to manager
		auto itr = mSprites.emplace( spriteName, sprite );
		if( itr.first == mSprites.end() ) {
			std::cerr << "SpriteManager::AddSprite() - Failed to add sprite: " <<
				spriteName << std::endl;
			return nullptr;
		}
		spritePtr = &itr.first->second;
	}
	return spritePtr;
}

Sprite* SpriteManager::GetSprite( const std::string &spriteName ) const {
	Sprite *sprite = nullptr;
	auto itr = mSprites.find( spriteName );
	if( itr != mSprites.end() ) {
		sprite = const_cast<Sprite*>( &itr->second );
	}
	return sprite;
}