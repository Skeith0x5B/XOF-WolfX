/*
===============================================================================

	XOF
	===
	File	:	XOF_GraphicsDevice_OpenGL.hpp
	Desc	:	OpenGL implementation of the GraphicsDevice interface.

				NOTE: Add ability to enable/disable certain vertex attributes?

===============================================================================
*/
#ifndef XOF_RENDERER_OPENGL_HPP
#define XOF_RENDERER_OPENGL_HPP


#include "../Platform/XOF_Platform.hpp"
#include "../GameplayFoundations/XOF_GameObject.hpp"
// TEMP
struct TempLevel;


struct RendererState {
	// wireframe/solid
	// winding order
	// cull
	// etc..?
};


class Renderer {
public:
	bool	StartUp();
	void	ShutDown();

	void	Draw();
	void	Draw( const GameObject& object );

	// void SubmitBatchRequest(...);
	// void RenderFrameAndSwapBuffers();

	void	ClearScreen();
	void	Resize( U32 newWidth, U32 newHeight );

	//		TEMP
	void	DrawTempWolfXLevel( const TempLevel& tempLevel );
			// Access sprite, update pre-existing draw to decide between sprite and mesh
	void	DrawTempWolfXGameObject( const GameObject& object, const FirstPersonCamera& camera  );

//private:
	void	DrawMesh( const Mesh& mesh, const Material& material );
	void	DrawSprite( const Sprite& sprite, const Material& material );
	//		RenderRequestList;
	//		DynamicPoolAllocator<RenderRequest>;
};


#endif // XOF_RENDERER_OPENGL_HPP