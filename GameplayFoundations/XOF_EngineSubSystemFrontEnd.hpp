/*
===============================================================================

	XOF
	===
	File	:	XOF_EngineSubSystemFrontEnd.hpp
	Desc	:	Provides a uniform front-end for game objects to use
				when they need to interact with the underlying engine subsystems.

===============================================================================
*/
#ifndef XOF_ENGINE_SUBSYSTEM_FRONT_END_HPP
#define XOF_ENGINE_SUBSYSTEM_FRONT_END_HPP


// TODO: Replace api-specific includes with interface/api-agnostic includes
#include "../Rendering/XOF_Renderer_OpenGL.hpp"
#include "../Audio/XOF_AudioEngine.hpp"


struct EngineSubSystems {
	EngineSubSystems() {
		renderer = nullptr;
		audio = nullptr;
	}

	std::unique_ptr<Renderer>		renderer;
	std::unique_ptr<AudioEngine>	audio;
	// etc...
};


#endif // XOF_ENGINE_SUBSYSTEM_FRONT_END_HPP