/*
===============================================================================

	XOF0
	===
	File	:	XOF_GraphicsDevice_OpenGL.cpp
	Desc	:	OpenGL implementation of the GraphicsDevice interface.

				glEnable( GL_FRAMEBUFFER_SRGB ); undefined error/issues
				were resolved by getting glew (use this in future, never
				rely on Microsoft's implementation - ever.)

===============================================================================
*/
#include "XOF_Renderer_OpenGL.hpp"
#include "XOF_Vertex.hpp"
#include "XOF_GPUState.hpp"

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>


#include "../GameplayFoundations/XOF_GameObject.hpp"
// define temp level
#include "../Game/WolfX.hpp"


// Must correspond to those in XOF_GPUState.hpp
static U32 XOF_GL_GPU_STATE_FLAGS[] = {
	GL_CW,
	GL_CCW,
	GL_CULL_FACE,
	GL_BACK,
	GL_FRONT,
	GL_DEPTH_TEST,
	GL_DEPTH_CLAMP,
	// etc..?
};


bool Renderer::StartUp() {
	GLenum status = glewInit();
	if( status != GLEW_OK ) {
		std::cerr << "Error: Glew failed to initialise!" << std::endl;
		return false;
	}

	glClearColor( 0.5f, 0.5f, 0.5f, 0.f );

	glFrontFace( GL_CW );
	glCullFace( GL_BACK );
	glEnable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_DEPTH_CLAMP );
	// depthMask, depthFunc, depthRange, clearDepth etc...

	// Transparency
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// Wire-frame
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// Bind these here since XOF uses the same vertex format for
	// all meshes and sprites.
	glEnableVertexAttribArray( 0 ); // position
	glEnableVertexAttribArray( 1 ); // normal
	glEnableVertexAttribArray( 2 ); // texture-coordinates
	glEnableVertexAttribArray( 3 ); // tangent

	return true;
}

void Renderer::ShutDown() {
	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
	glDisableVertexAttribArray( 3 );
}

void Renderer::SubmitRenderRequest( RenderRequest& request ) {
	mRenderRequests.push_back( std::move( request ) );
}

void Renderer::RenderFrame() {
	ClearScreen();

	for( RenderRequest& req : mRenderRequests ) {
		glm::mat4 MVP = ( req.camera->GetProjectionMatrix() * req.camera->GetViewMatrix() ) * 
			req.transform->GetModelToWorldMatrix();

		req.material->GetShader()->SetUniform( "transform", MVP );

		SetGPUState( *req.material->GetGPUState() );

		switch( req.type ) {
			case XOF_RENDER_REQUEST_TYPE::RENDER_MESH: DrawMesh( *req.mesh, *req.material ); break;
			case XOF_RENDER_REQUEST_TYPE::RENDER_SPRITE: DrawSprite( *req.sprite, *req.material ); break;
		}
	}

	mRenderRequests.clear();
}

void Renderer::ClearScreen() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Renderer::Resize( U32 newWidth, U32 newHeight ) {
	glViewport( 0, 0, newWidth, newHeight );
}

void Renderer::DrawMesh( const Mesh& mesh, const Material& material ) {
	U32 diffuseTextureCount = material.GetTextureCount( XOF_TEXTURE_TYPE::DIFFUSE );
	U32 normalMapCount = material.GetTextureCount( XOF_TEXTURE_TYPE::NORMAL );

	U32 subMeshCount = mesh.GetSubMeshCount();
	for( U32 i=0; i < subMeshCount; ++i ) {
		glBindBuffer( GL_ARRAY_BUFFER, mesh.GetSubMeshData()[i].VB );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 12 ) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 24 ) );
		glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 32 ) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mesh.GetSubMeshData()[i].IB );

		const U32 materialIndex = mesh.GetSubMeshData()[i].materialIndex;
		if( materialIndex < diffuseTextureCount ) {
			material.GetTexture( XOF_TEXTURE_TYPE::DIFFUSE, materialIndex )->Bind( 0 );
		}

		if( materialIndex < normalMapCount ) {
			material.GetTexture( XOF_TEXTURE_TYPE::NORMAL, materialIndex )->Bind( 1 );
		}

		glDrawElements( GL_TRIANGLES, mesh.GetSubMeshData()[i].indexCount, GL_UNSIGNED_INT, 0 );
	}

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Renderer::DrawSprite( const Sprite& sprite, const Material& material ) {
	glBindBuffer( GL_ARRAY_BUFFER, sprite.mVB );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Vertex ) * 4, &sprite.mVertices, GL_DYNAMIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), 0 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 12 ) );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 24 ) );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), reinterpret_cast<GLvoid*>( 32 ) );

	// TODO: Material index checking
	const U32 materialIndex = 0;
	material.GetTexture( XOF_TEXTURE_TYPE::DIFFUSE, materialIndex )->Bind( 0 );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, sprite.mIB );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void Renderer::SetGPUState( const GPUState& gpuState ) {
	U8 highestFlag = gpuState.GetHighestEnabledFlag();
	U16 flags = gpuState.GetEnabledFlags();
	for( U16 i=0; i <= highestFlag; ++i ) {
		if( flags & ( 1 << i ) ) {
			glEnable( XOF_GL_GPU_STATE_FLAGS[i] );
		}
	}

	highestFlag = gpuState.GetHighestDisabledFlag();
	flags = gpuState.GetDisabledFlags();
	for( U16 i=0; i <= highestFlag; ++i ) {
		if( flags & ( 1 << i ) ) {
			glDisable( XOF_GL_GPU_STATE_FLAGS[i] );
		}
	}
}