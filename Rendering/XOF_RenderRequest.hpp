/*
===============================================================================

	XOF
	===
	File	:	XOF_RenderRequest.hpp
	Desc	:	Allows game objects to make requests of the renderer,
				used to implement basic batched upating. Objects submit
				requests then the game loop tells the renderer to render
				a frame, wherein it processes requests, calling the appropriate
				draw functions.

				GRAPHICS API AGNOSTIC. API SPECIFIC FLAGS ARRAY TO BE ADDED
				TO RENDERER CLASS/IMPLEMENTATION.

===============================================================================
*/
#ifndef XOF_RENDER_REQUEST_HPP
#define XOF_RENDER_REQUEST_HPP


#include "../Platform/XOF_Platform.hpp"
class Material;
class Mesh;
class Sprite;
class FirstPersonCamera;
struct Transform;


enum XOF_RENDER_REQUEST_TYPE {
	RENDER_MESH = 0,
	RENDER_SPRITE,
	INVALID_RENDERING_REQUEST,
	// RENDER STRING etc...
};


// Material pointer non-const since we will be setting shader variables
struct RenderRequest {
	// Default ctor, for use in request container only
	RenderRequest() {
		type = XOF_RENDER_REQUEST_TYPE::INVALID_RENDERING_REQUEST;
		material = nullptr;
		mesh = nullptr;
		sprite = nullptr;
		camera = nullptr;
		transform = nullptr;
	}

	RenderRequest( XOF_RENDER_REQUEST_TYPE type, Material *material,
		const Mesh *mesh, const Sprite *sprite, const FirstPersonCamera *camera, 
		const Transform *transform ) {
		XOF_ASSERT( type >= 0 && type < XOF_RENDER_REQUEST_TYPE::INVALID_RENDERING_REQUEST )
		this->type = type;
		this->material = material;

		switch( type ) {
			case XOF_RENDER_REQUEST_TYPE::RENDER_MESH: this->mesh = mesh; break;
			case XOF_RENDER_REQUEST_TYPE::RENDER_SPRITE: this->sprite = sprite; break;
		}

		this->camera = camera;
		this->transform = transform;
	}

	RenderRequest( RenderRequest&& rhs ) {
		XOF_ASSERT( rhs.type >= 0 && rhs.type < XOF_RENDER_REQUEST_TYPE::INVALID_RENDERING_REQUEST )
		// Steal the data from rhs
		this->type = rhs.type;
		this->material = rhs.material;
		
		switch( type ) {
			case XOF_RENDER_REQUEST_TYPE::RENDER_MESH: this->mesh = rhs.mesh; break;
			case XOF_RENDER_REQUEST_TYPE::RENDER_SPRITE: this->sprite = rhs.sprite; break;
		}

		this->camera = rhs.camera;
		this->transform = rhs.transform;

		rhs.type = XOF_RENDER_REQUEST_TYPE::INVALID_RENDERING_REQUEST;
		rhs.material = nullptr;
		rhs.mesh = nullptr;
		rhs.sprite = nullptr;
		rhs.camera = nullptr;
		rhs.transform = nullptr;
	}


	XOF_RENDER_REQUEST_TYPE		type;
	
	Material					*material;
	union {
		const Mesh				*mesh;
		const Sprite			*sprite;
	};

	// TODO: Replace with camera base class
	const FirstPersonCamera		*camera;
	const Transform				*transform;
};


#endif // XOF_RENDER_REQUEST_HPP