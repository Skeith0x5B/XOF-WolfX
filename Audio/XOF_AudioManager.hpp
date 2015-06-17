/*
===============================================================================

	XOF
	===
	File	:	XOF_AudioManager.hpp
	Desc	:	Loads and manages music and sound effects.

===============================================================================
*/
#ifndef XOF_AUDIO_MANAGER_HPP
#define XOF_AUDIO_MANAGER_HPP


#include "../Platform/XOF_Platform.hpp"
#include "../Audio/XOF_Music.hpp"
#include <unordered_map>
class SoundEffect;


class AudioManager {
public:
	static AudioManager*	Get();

	bool					StartUp();
	void					ShutDown();
	
	SoundEffect*			AddSoundEffect( const std::string &soundEffectName );
	SoundEffect*			GetSoundEffect( const std::string &soundEffectName ) const;

							// Since music is streamed, we don't 'store' them like
							// sound effects; they're streamed instead
	Music*					GetMusic( const std::string& musicName );

private:
	// TODO: Replace string with hash value? Container already hashes?
	// music...
	std::unordered_map<std::string, SoundEffect>	mSoundEffects;
	Music											mMusic;
};


#endif // XOF_AUDIO_MANAGER_HPP