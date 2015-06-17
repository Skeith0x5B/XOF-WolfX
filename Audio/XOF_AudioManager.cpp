/*
===============================================================================

	XOF
	===
	File	:	XOF_AudioManager.hpp
	Desc	:	Loads and manages music and sound effects.

===============================================================================
*/
#include "XOF_AudioManager.hpp"
#include "XOF_Music.hpp"
#include "XOF_SoundEffect.hpp"


AudioManager* AudioManager::Get() {
	static AudioManager singletonInstance;
	return &singletonInstance;
}

bool AudioManager::StartUp() {
	return true;
}

void AudioManager::ShutDown() {
	mSoundEffects.clear();
}
	
SoundEffect* AudioManager::AddSoundEffect( const std::string &soundEffectName ){
	SoundEffect *soundEffectPtr = GetSoundEffect( soundEffectName );
	if( !soundEffectPtr ) {
		// Setup a sound effect
		SoundEffect soundEffect( soundEffectName );

		// Add to manager
		auto itr = mSoundEffects.emplace( soundEffectName, std::move( soundEffect ) );
		if( itr.first == mSoundEffects.end() ) {
			std::cerr << "AudioManager::AddSoundEffect() - Failed to add sound effect: " <<
				soundEffectName << std::endl;
			return nullptr;
		}
		soundEffectPtr = &itr.first->second;
	}
	return soundEffectPtr;
}

SoundEffect* AudioManager::GetSoundEffect( const std::string &soundEffectName ) const {
	SoundEffect *soundEffect = nullptr;
	auto itr = mSoundEffects.find( soundEffectName );
	if( itr != mSoundEffects.end() ) {
		soundEffect = const_cast<SoundEffect*>( &itr->second );
	}
	return soundEffect;
}

Music* AudioManager::GetMusic( const std::string& musicName ) {
	mMusic.Load( musicName );
	if( !mMusic.IsLoaded() ) {
		return nullptr;
	}
	return &mMusic;
}