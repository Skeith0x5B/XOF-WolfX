/*
===============================================================================

	XOF
	===
	File	:	XOF_SoundEffect.cpp
	Desc	:	Describes a basic sound effect.

===============================================================================
*/
#include "XOF_SoundEffect.hpp"


SoundEffect::SoundEffect() {
	mIsLoaded = false;
}

SoundEffect::SoundEffect( SoundEffect&& rhs ) {
	this->mIsLoaded = rhs.mIsLoaded;
	this->mTempBuffer = rhs.mTempBuffer;
	this->mSound = rhs.mSound;

	mSound.setBuffer( mTempBuffer );

	rhs.mIsLoaded = false;
}

SoundEffect::SoundEffect( const std::string& fileName ) {
	Load( fileName );
}

void SoundEffect::Load( const std::string& fileName ) {
	mIsLoaded = mTempBuffer.loadFromFile( fileName );
	if( mIsLoaded ) {
		mSound.setBuffer( mTempBuffer );
		mSound.setVolume( 50.f );
	}
}

bool SoundEffect::IsLoaded() const { 
	return mIsLoaded; 
}

void SoundEffect::Play() { 
	mSound.play(); 
}

void SoundEffect::Pause() { 
	mSound.pause(); 
}

void SoundEffect::Stop() { 
	mSound.stop(); 
}

void SoundEffect::Restart() { 
	mSound.stop();
	mSound.play(); 
}