#ifndef WOLFX_HPP
#define WOLFX_HPP


#include "XOF_Game.hpp"
// TODO: Replace these implementation-specific includes with the abstract ones
#include "../HIDs/XOF_Keyboard_SFML.hpp"
#include "../HIDs/XOF_Mouse_SFML.hpp"
#include "../Rendering/XOF_Renderer_OpenGL.hpp"
#include "../Rendering/XOF_FirstPersonCamera.hpp"


struct TempLevel {
	Texture	levelMap;
	Mesh levelGeom;
	UniqueMeshPtr levelPtr;
	std::vector<Transform> tileTransforms;
	Material *tileMaterial;
	FirstPersonCamera *playerView;
};


class WolfX : public Game {
public:
						WolfX();
	virtual				~WolfX();

	virtual bool		Initialise( U16 width, U16 height, CHAR* caption ) override;	
	virtual void		Run();

private:
	Renderer			mRenderer;
	Keyboard			mKeyboard;
	Mouse				mMouse;

	virtual void		SetupScene();

	virtual void		HandleEvents();
	virtual void		HandleInput();
	virtual void		Render();
};


#endif // WOLFX_HPP