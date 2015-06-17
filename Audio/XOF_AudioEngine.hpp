/*
===============================================================================

	XOF
	===
	File	:	XOF_AudioEngine.hpp
	Desc	:	Audio equivalent of the renderer, processes audio requests.

===============================================================================
*/
#ifndef XOF_AUDIO_ENGINE_HPP
#define XOF_AUDIO_ENGINE_HPP


#include "../Platform/XOF_Platform.hpp"
#include "XOF_AudioManager.hpp"
#include "XOF_AudioRequest.hpp"


class AudioEngine {
public:
	bool	Startup();
	void	Shutdown();

	void	SubmitAudioRequest( AudioRequest& request );
	void	Update();

private:
	AudioManager				*mAudioManager;
	std::vector<AudioRequest>	mAudioRequests;

	bool IsSoundEffectRequest( XOF_AUDIO_REQUEST_TYPE request );
};


#endif // XOF_AUDIO_ENGINE_HPP