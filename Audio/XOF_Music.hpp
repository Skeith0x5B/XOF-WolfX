/*
===============================================================================

	XOF
	===
	File	:	XOF_Music.hpp
	Desc	:	Basic music class, intended for background music streamed in
				from memory.

===============================================================================
*/
#ifndef XOF_MUSIC_HPP
#define XOF_MUSIC_HPP


#include "../Platform/XOF_Platform.hpp"
#include <SFML/Audio.hpp>


class Music {
public:
				Music();
				Music( const std::string& fileName );

	void		Load( const std::string& fileName );
	bool		IsLoaded() const;

	void		Play();
	void		Pause();
	void		Stop();
	void		Restart();

private:
	bool		mIsLoaded;
	sf::Music	mMusic;
};


#endif // XOF_MUSIC_HPP