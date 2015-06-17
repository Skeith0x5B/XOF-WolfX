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
//#include "../GameplayFoundations/XOF_GameObject.hpp"
#include "XOF_RenderRequest.hpp"
#include <vector>
class GPUState;
// TEMP
struct TempLevel;


class Renderer {
public:
	bool	StartUp();
	void	ShutDown();

	void	SubmitRenderRequest( RenderRequest& request );
	void	RenderFrame();

	void	ClearScreen();
	void	Resize( U32 newWidth, U32 newHeight );

private:
	//		TODO: Add a dynamic pool allocator here and add a next field to render request?
	//		Why is a queue the only container that doesn't crash? Perhaps because
	//		default ctor isn't used?
	std::vector<RenderRequest>	mRenderRequests;

	void	DrawMesh( const Mesh& mesh, const Material& material );
	void	DrawSprite( const Sprite& sprite, const Material& material );
	void	SetGPUState( const GPUState& gpuState );
};


#endif // XOF_RENDERER_OPENGL_HPP