/*
===============================================================================

	XOF
	===
	File	:	XOF_Game.cpp
	Desc	:	Basic application/game class, any project-specific game/gameplay/
				test scene must derive from this and implement.

===============================================================================
*/
#include "XOF_Game.hpp"

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "../Platform/XOF_Platform.hpp"
#include "../Platform/XOF_PlatformInfo.hpp"


Game::Game() 
: mWindow(nullptr)
, mIsInitialised( false ) {
}

Game::~Game() {
	if( mWindow ) {
		mWindow.reset(nullptr);
	}
}

// NOTE: Move context set-up to graphics device? Want all open-gl/directX stuff to stay in the rendering folder, not spread out
bool Game::Initialise( U16 width, U16 height, CHAR* caption ) {
	// Inspect OpenGL context/settings
	sf::ContextSettings context;
	context.majorVersion		= 4;
	context.minorVersion		= 4;
	context.antialiasingLevel	= 4;
	context.depthBits			= 24;
	context.stencilBits			= 8;

	mWindow.reset(new (std::nothrow)sf::Window( sf::VideoMode( width, height ), caption, 
												sf::Style::Default, context ) );

	if( !mWindow ) {
		return false;
	} else {
		GetPlatformInfo();
		std::cout << "\n";

		context = mWindow->getSettings();
		std::cout << "OpenGL version: " << context.majorVersion << "." << context.minorVersion << std::endl;
		std::cout << "Anti-Aliasing: " << context.antialiasingLevel << std::endl;
		std::cout << "Depth: " << context.depthBits << std::endl;
		std::cout << "Stencil: " << context.stencilBits << "\n" << std::endl;
	}

	// It's the job the deriving class to set mIsInitialised to true.
	// Just because Game::Init completes doesn't mean all the setup is done yet.
	return true;
}