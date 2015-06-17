/*
===============================================================================

	XOF
	===
	File	:	XOF_GameObject.hpp
	Desc	:	The base for all objects/entities in the game world.

				NOTE: GameObjectManager can effectively "despawn" game objects
						by removing them from it's internal container.

===============================================================================
*/
#ifndef XOF_GAME_OBJECT_HPP
#define XOF_GAME_OBJECT_HPP


// NOTE: ADD CAMERA?


#include "../GameplayFoundations/XOF_Event.hpp"
// ---
#include "../Rendering/XOF_Transform.hpp"
#include "../Rendering/XOF_Material.hpp"
// Collision volume include
#include "../Rendering/XOF_Mesh.hpp"
#include "../Rendering/XOF_Sprite.hpp"
// TODO: Introduce general base for camera types
#include "../Rendering/XOF_FirstPersonCamera.hpp"
#include <glm/vec3.hpp>


#include "XOF_EngineSubSystemFrontEnd.hpp"


class GameObject {
public:
					// Add position, mesh/sprite, material?
					GameObject( EngineSubSystems *engineSystems, glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
	virtual			~GameObject();

	virtual	void	Update( float dt );
	virtual void	OnEvent( const Event *_event );

	std::string&	GetName() const;
	I32				GetType() const;

	void			SetMesh( const Mesh *mesh );
	void			SetMeshCopy( const Mesh *mesh );
	//void			DestroyMeshCopy();
	Mesh*			GetMesh() const;

	void			SetSprite( const Sprite *sprite );
	void			SetSpriteCopy( const Sprite *sprite );
	//void			DestroySpriteCopy();
	Sprite*			GetSprite() const;

	void			SetMaterial( const Material *material );
	Material*		GetMaterial() const;

	glm::vec3*		GetPosition() const;

	void			Translate( const glm::vec3& translation );
	void			Rotate( float aboutX, float aboutY, float aboutZ );
	void			Scale( float alongX, float alongY, float alongZ );
	Transform&		GetTransform() const;

protected:
	std::string		mName;
	I32				mType;

	Transform		mTransform;
	//union {
		Mesh*			mMesh;
		Sprite*			mSprite;
	//};
	Material*		mMaterial;
	//				Collision Volume
	//Camera*		mCamera; CAMERAS WOULD NEED TO USE INHERITANCE
					// Used when the object has it's own copy
					// of a mesh or spirte (though normally
					// a mesh/sprite will be provided by the appropriate manager).
	bool			mResponsibleForMesh;
	bool			mResponsibleForSprite;

	// TEMP - REPLACE WITH CAMERA-BASE CLASS?
	FirstPersonCamera	*mCamera;

	// Engine subsystems
	EngineSubSystems	*mEngine;
	Renderer*			mEngineRenderer;
	//MeshManager*		mEngineMeshManager;
	//SpriteManager*	mEngineSpriteManager;
	//MaterialManager*	mEngineMaterialManager;
	//Audio*			mAudioSystem;
	// etc...
};


using UniqueGameObjectPtr = std::unique_ptr<GameObject>;


#endif // XOF_GAME_OBJECT_HPP