/*
===============================================================================

	XOF
	===
	File	:	XOF_AudioEngine.cpp
	Desc	:	Audio equivalent of the renderer, processes audio requests.

===============================================================================
*/
#include "XOF_AudioEngine.hpp"
#include "XOF_SoundEffect.hpp"
#include "XOF_Music.hpp"


bool AudioEngine::Startup() { 
	mAudioManager = AudioManager::Get();
	return true; 
}

void AudioEngine::Shutdown() { 
	mAudioRequests.clear(); 
}

void AudioEngine::SubmitAudioRequest( AudioRequest& request ) {
	mAudioRequests.push_back( std::move( request ) );
}

void AudioEngine::Update() {
	for( AudioRequest& req : mAudioRequests ) {
		// Dealing with a sound effect or music
		if( IsSoundEffectRequest( req.type ) ) {
			if( req.soundEffect == nullptr ) {
				req.soundEffect = mAudioManager->GetSoundEffect( req.audioFileName );
				XOF_ASSERT( req.soundEffect != nullptr )
			}
		} else if( req.music == nullptr ) {
			req.music = mAudioManager->GetMusic( req.audioFileName );
			XOF_ASSERT( req.music != nullptr )
		}

		switch( req.type ) {
			case PLAY_SOUND_EFFECT: req.soundEffect->Play(); break;
			case PLAY_MUSIC: req.music->Play(); break;
		}
	}
	mAudioRequests.clear();
}

bool AudioEngine::IsSoundEffectRequest( XOF_AUDIO_REQUEST_TYPE request ) {
	return ( request >= PLAY_SOUND_EFFECT && request <= RESTART_SOUND_EFFECT );
}