/*
===============================================================================

	XOF
	===
	File	:	XOF_Music.cpp
	Desc	:	Basic music class, intended for background music streamed in
				from memory.

===============================================================================
*/
#include "XOF_Music.hpp"


Music::Music() { 
	mIsLoaded = false; 
}

Music::Music( const std::string& fileName ) {
	Load( fileName );
}

void Music::Load( const std::string& fileName ) { 
	mIsLoaded = mMusic.openFromFile( fileName );
	mMusic.setVolume( 50.f ); 
}

bool Music::IsLoaded() const { 
	return mIsLoaded; 
}

void Music::Play() { 
	mMusic.play();
}

void Music::Pause() { 
	mMusic.pause(); 
}

void Music::Stop() { 
	mMusic.stop(); 
}

void Music::Restart() {
	mMusic.stop();
	mMusic.play();
}