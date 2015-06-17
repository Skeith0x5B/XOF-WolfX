/*
===============================================================================

	XOF
	===
	File	:	XOF_SoundEffect.hpp
	Desc	:	Describes a basic sound effect.

===============================================================================
*/
#ifndef XOF_SOUND_EFFECT_HPP
#define XOF_SOUND_EFFECT_HPP


#include "../Platform/XOF_Platform.hpp"
#include <SFML/Audio.hpp>


class SoundEffect {
public:
					SoundEffect();
					SoundEffect( SoundEffect&& rhs );
					SoundEffect( const std::string& fileName );

	void			Load( const std::string& fileName );
	bool			IsLoaded() const;

	void			Play();
	void			Pause();
	void			Stop();
	void			Restart();

private:
	bool			mIsLoaded;
					// Buffer probably can't be temporary, buffer and sound come as a pair
	sf::SoundBuffer	mTempBuffer;
	sf::Sound		mSound;
};


#endif // XOF_SOUND_EFFECT_HPP
