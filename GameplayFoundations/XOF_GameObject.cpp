/*
===============================================================================

	XOF
	===
	File	:	XOF_GameObject.cpp
	Desc	:	The base for all objects/entities in the game world.

===============================================================================
*/
#include "XOF_GameObject.hpp"


static const I32 XOF_GAME_OBJECT_TYPE_UNDEFINED = -1;
static const U32 XOF_UNNAMED_GAME_OBJECT_COUNT = 0;


GameObject::GameObject( glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, 
	Material *material, FirstPersonCamera *camera ) {
	mTransform.translation = spawnPos;
	mMesh = mesh;
	mSprite = sprite;
	mMaterial = material;
	mCamera = camera;
}

GameObject::~GameObject() {
	if( mResponsibleForMesh ) {
		delete mMesh;
	}
	if( mResponsibleForSprite ) {
		delete mSprite;
	}
}

void GameObject::Update( float dt ) {
	// DO NOTHING - Override in game-specific classes
}

void GameObject::OnEvent( const Event *_event ) {
	// DO NOTHING - Override in game-specific classes
}

std::string& GameObject::GetName() const {
	return const_cast<std::string&>( mName );
}

I32 GameObject::GetType() const {
	return mType;
}

void GameObject::SetMesh( const Mesh* mesh ) {
	mMesh = const_cast<Mesh*>( mesh );
}

void GameObject::SetMeshCopy( const Mesh *mesh ) {
	mMesh = const_cast<Mesh*>( mesh );
	mResponsibleForMesh = true;
}

Mesh* GameObject::GetMesh() const {
	return const_cast<Mesh*>( mMesh );
}

void GameObject::SetSprite( const Sprite *sprite ) {
	mSprite = const_cast<Sprite*>( sprite );
}
void GameObject::SetSpriteCopy( const Sprite *sprite ) {
	mSprite = const_cast<Sprite*>( sprite );
	mResponsibleForSprite = true;
}

Sprite* GameObject::GetSprite() const {
	return const_cast<Sprite*>( mSprite );
}

void GameObject::SetMaterial( const Material *material ) {
	mMaterial = const_cast<Material*>( material );
}

Material* GameObject::GetMaterial() const {
	return mMaterial;
}

void GameObject::Translate( const glm::vec3& translation ) {
	mTransform.translation += translation;
}

void GameObject::Rotate( float aboutX, float aboutY, float aboutZ ) {
	mTransform.rotation = glm::vec3( aboutX, aboutY, aboutZ );
}

void GameObject::Scale( float alongX, float alongY, float alongZ ) {
	mTransform.scale = glm::vec3( alongX, alongY, alongZ );
}

Transform& GameObject::GetTransform() const {
	return const_cast<Transform&>( mTransform );
}