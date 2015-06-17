/*
===============================================================================

	XOF
	===
	File	:	XOF_AudioRequest.hpp
	Desc	:	Allows game objects to make requests of the audio system,
				used to implement basic batched upating. Objects submit
				requests then the game loop tells the audio system to 
				processes requests, calling the appropriate audio functions.

				AUDIO API AGNOSTIC. API SPECIFIC FLAGS ARRAY TO BE ADDED
				TO RENDERER CLASS/IMPLEMENTATION.

===============================================================================
*/
#ifndef XOF_AUDIO_REQUEST_HPP
#define XOF_AUDIO_REQUEST_HPP


#include "../Platform/XOF_Platform.hpp"
class Music;
class SoundEffect;


enum XOF_AUDIO_REQUEST_TYPE {
	// Music
	PLAY_MUSIC = 0,
	PAUSE_MUSIC,
	STOP_MUSIC,
	RESTART_MUSIC,
	// Sound FX
	PLAY_SOUND_EFFECT,
	PAUSE_SOUND_EFFECT,
	STOP_SOUND_EFFECT,
	RESTART_SOUND_EFFECT,
	INVALID_AUDIO_REQUEST,
	// etc...
};


struct AudioRequest {
	// Default ctor, for use in request container only
	AudioRequest() {
		type = XOF_AUDIO_REQUEST_TYPE::INVALID_AUDIO_REQUEST;
		music = nullptr;
		soundEffect = nullptr;
		volume = 0.f;
	}

	AudioRequest( XOF_AUDIO_REQUEST_TYPE type, Music *music, 
		SoundEffect *soundEffect, float volume, const std::string& audioFileName ) {
		XOF_ASSERT( type >= 0 && type < XOF_AUDIO_REQUEST_TYPE::INVALID_AUDIO_REQUEST )
		this->type = type;
		
		if( type >= 0 && type <= XOF_AUDIO_REQUEST_TYPE::RESTART_MUSIC ) {
			this->music = music;
		} else if( type >= XOF_AUDIO_REQUEST_TYPE::PLAY_SOUND_EFFECT &&
			type <= XOF_AUDIO_REQUEST_TYPE::RESTART_SOUND_EFFECT ) {
			this->soundEffect = soundEffect;
		}

		this->volume = volume;
		this->audioFileName = audioFileName;
	}

	AudioRequest( AudioRequest&& rhs ) {
		XOF_ASSERT( rhs.type >= 0 && rhs.type < XOF_AUDIO_REQUEST_TYPE::INVALID_AUDIO_REQUEST )
		this->type = rhs.type;

		if( type >= 0 && type <= XOF_AUDIO_REQUEST_TYPE::RESTART_MUSIC ) {
			this->music = rhs.music;
		} else if( type >= XOF_AUDIO_REQUEST_TYPE::PLAY_SOUND_EFFECT &&
			type <= XOF_AUDIO_REQUEST_TYPE::RESTART_SOUND_EFFECT ) {
			this->soundEffect = rhs.soundEffect;
		}

		this->volume = rhs.volume;
		this->audioFileName = rhs.audioFileName;
	}

	XOF_AUDIO_REQUEST_TYPE	type;

	union {
		Music				*music;
		SoundEffect			*soundEffect;
	};

	float					volume;

	std::string				audioFileName;
};


#endif // XOF_AUDIO_REQUEST_HPP