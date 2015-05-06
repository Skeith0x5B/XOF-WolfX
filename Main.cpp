/*
===============================================================================

	XOF
	===
	File	:	Main.cpp
	Desc	:	Program start point.

===============================================================================
*/
#include <iostream>

#include "Platform/XOF_Platform.hpp"
#include "Game/WolfX.hpp"


int main( int argc, char **argv ) {
	std::cout << "XOF\n---" << std::endl;

	WolfX game;
	if( game.Initialise( 800, 600, "XOF (OpenGL) - WolfX" ) ) {
		game.Run();
	} else {
		std::cout << "XOF Could not initialise." << std::endl;
	}

	return 0;
}