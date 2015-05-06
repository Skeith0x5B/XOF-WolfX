/*
===============================================================================

	XOF
	===
	File	:	XOF_Game.hpp
	Desc	:	Basic application/game class, any project-specific game/gameplay/
				test scene must derive from this and implement.

===============================================================================
*/
#ifndef XOF_GAME_HPP
#define XOF_GAME_HPP


#include "../Platform/XOF_Platform.hpp"


namespace sf {
	class Window;
}


class Game {
public:
								Game();
	virtual						~Game();

	virtual bool				Initialise( U16 width, U16 height, CHAR* caption );	
	virtual void				Run() = 0;

protected:
	bool						mIsInitialised;

	std::unique_ptr<sf::Window> mWindow;

	virtual void				SetupScene() = 0;

	virtual void				HandleEvents() = 0;
	virtual void				HandleInput() = 0;
	virtual void				Render() = 0;
};


#endif // XOF_GAME_HPP