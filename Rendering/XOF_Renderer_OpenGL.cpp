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

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>


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

void Renderer::Draw() {
	// ...
}

void Renderer::Draw( const GameObject& object ) {
	DrawMesh( *object.GetMesh(), *object.GetMaterial() );
}

void Renderer::ClearScreen() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Renderer::Resize( U32 newWidth, U32 newHeight ) {
	glViewport( 0, 0, newWidth, newHeight );
}

// TEMP
#include "../Game/WolfX.hpp"
void Renderer::DrawTempWolfXLevel( const TempLevel& tempLevel ) {
	const glm::mat4 MVP = tempLevel.playerView->GetProjectionMatrix() * tempLevel.playerView->GetViewMatrix();
	tempLevel.tileMaterial->GetShader()->SetUniform( "transform", 
			MVP * tempLevel.tileTransforms[0].GetModelToWorldMatrix() );

	DrawMesh( tempLevel.levelGeom, *tempLevel.tileMaterial );
}

void Renderer::DrawTempWolfXGameObject( const GameObject& object, const FirstPersonCamera& camera ) {
	glm::mat4 MVP = ( camera.GetProjectionMatrix() * camera.GetViewMatrix() ) * 
		object.GetTransform().GetModelToWorldMatrix();
	object.GetMaterial()->GetShader()->SetUniform( "transform", MVP );
	if( object.GetSprite() != nullptr) {
		DrawSprite( *object.GetSprite(), *object.GetMaterial() );
	} else if( object.GetMesh() != nullptr) {
		DrawMesh( *object.GetMesh(), *object.GetMaterial() );
	}
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
	glDisable( GL_CULL_FACE );

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

	glEnable( GL_CULL_FACE );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}