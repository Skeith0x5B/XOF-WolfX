/*
===============================================================================

	XOF
	===
	File	:	XOF_Event.hpp
	Desc	:	The base for all events in the game.

===============================================================================
*/
#ifndef XOF_EVENT_HPP
#define XOF_EVENT_HPP


#include "../Platform/XOF_Platform.hpp"


// Generic event argument
struct EventArg {
	// engine book puts a type enum here?
	union {
		U32 asU32;
		// GLM vec3 has unallowed operation...
		float asVec3f[3];
		float asFloat;
	};
};


struct Event {
	static const U8 MAX_EVENT_ARGS = 8;

	U32 type;
	// Add hashing algorithm then switch to this
	//std::string	type;
	EventArg arguments[MAX_EVENT_ARGS];
};


#endif // XOF_EVENT_HPP