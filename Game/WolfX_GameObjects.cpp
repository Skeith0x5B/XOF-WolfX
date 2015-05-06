#include "WolfX_GameObjects.hpp"
#include "WolfX_Events.hpp"


// PLAYER
Player::Player( glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, Material *material, FirstPersonCamera *camera ) :
	GameObject( spawnPos, mesh, sprite, material, camera ) {
}

Player::~Player() {
}

void Player::Update( float dt ) {
	//Renderer->Draw( *this ); // MOVE TO REQUEST + BATCH SYSTEM
}

void Player::OnEvent( const Event* _event ) {
	switch( _event->type ) {
		case WOLFX_EVENTS::PLAYER_SPAWNED: 
			std::cout << "Player::OnEvent() - PLAYER_SPAWNED" << std::endl; 
			break;
		case WOLFX_EVENTS::PLAYER_MOVED: 
			mCamera->Translate( _event->arguments[PlayerMovedEvent::ARGS::TRANSLATION].asVec3f[0],
								_event->arguments[PlayerMovedEvent::ARGS::TRANSLATION].asVec3f[1],
							    _event->arguments[PlayerMovedEvent::ARGS::TRANSLATION].asVec3f[2] );
			// TODO: Merge camera Yaw() and Pitch() into PitchAndYaw() to reduce stack pushes
			mCamera->Yaw( _event->arguments[PlayerMovedEvent::ARGS::YAW].asFloat );
			mCamera->Pitch( _event->arguments[PlayerMovedEvent::ARGS::PITCH].asFloat );
			break;
	}
}
// ---------------------------------------------------------------------------


// BILLBOARD
BillBoard::BillBoard( glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, Material *material, FirstPersonCamera *camera ) :
	GameObject( spawnPos, mesh, sprite, material, camera ) {
}

BillBoard::~BillBoard() {
}

void BillBoard::Update( float dt ) {
}

void BillBoard::OnEvent( const Event *_event ) {
	switch( _event->type ) {
		case WOLFX_EVENTS::PLAYER_SPAWNED: break;
		case WOLFX_EVENTS::PLAYER_MOVED: 
			FacePlayer( mCamera->GetPosition() ); 
			break;
	}
}

void BillBoard::FacePlayer( const glm::vec3& cameraPos ) {
	static const glm::vec3 billboardPos = glm::vec3( 0.f, 0.f, 0.f ); //mTransform.translation;
	static const glm::vec3 const up = glm::vec3( 0.f, 1.f, 0.f );
	
	// Get vector from billboard position (billboards spawn at (0,0,0)
	// and are translated via mTransform)
	glm::vec3 look = cameraPos;// - billboardPos;
	look /= look.length();

	// Create billboard right and forward vectors
	glm::vec3 right = glm::cross( up, look );
	// Do not rotate as camera moves backward and forward
	right.z = 0;

	glm::vec3 forward = glm::cross( up, right );
	forward /= forward.length();

	// angle between camera position and forward
	float dot = glm::dot( forward, look );

	// add angle between camera view and forward
	look = mCamera->GetViewDirection() - billboardPos;
	forward = glm::cross( up, right );
	dot += glm::dot( forward, look );

	mTransform.rotation.y = dot;
}
// ---------------------------------------------------------------------------


// MOVER
Mover::Mover( const glm::vec3& directionToMove,	glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, 
	Material *material, FirstPersonCamera *camera) : GameObject( spawnPos, mesh, sprite, material, camera ) { 
	mDirectionToMoveAlong = directionToMove;
	mStartingPos = spawnPos;
	mState = MOVER_OBJECT_STATES::CLOSED;
	mMovementSpeed = 0.1f;
}

Mover::~Mover() {
}

void Mover::Update( float dt ) {
	mMovementSpeed = dt;
}

void Mover::OnEvent( const Event *_event ) {
	switch( _event->type ) {
		case WOLFX_EVENTS::PLAYER_SPAWNED: break;
		case WOLFX_EVENTS::PLAYER_MOVED: break;
 		case WOLFX_EVENTS::PLAYER_PRESSED_ACTION_KEY: Move(); break;
	}
}

void Mover::SetMovementDirection( const glm::vec3& direction ) {
	mDirectionToMoveAlong = direction;
}

void Mover::Move() {
	switch( mState ) {
		case OPEN:		break;
		case OPENING:	break;
		case CLOSING:	break;
		case CLOSED:	break;
	}
}
// ---------------------------------------------------------------------------


// ENEMY
Enemy::Enemy( glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, 
	Material *material, FirstPersonCamera *camera) : GameObject( spawnPos, mesh, sprite, material, camera ) { 
}

Enemy::~Enemy() {
}

void Enemy::Update( float dt ) {
	mSprite->StepAnimation( 0.001f );
}

void Enemy::OnEvent( const Event *_event ) {
	switch( _event->type ) {
		case WOLFX_EVENTS::PLAYER_SPAWNED: break;
		case WOLFX_EVENTS::PLAYER_MOVED: 
			FacePlayer( mCamera->GetPosition() ); 
			break;
	}
}

void Enemy::FacePlayer( const glm::vec3& cameraPos ) {
	static const glm::vec3 billboardPos = glm::vec3( 0.f, 0.f, 0.f ); //mTransform.translation;
	static const glm::vec3 const up = glm::vec3( 0.f, 1.f, 0.f );
	
	// Get vector from billboard position (billboards spawn at (0,0,0)
	// and are translated via mTransform)
	glm::vec3 look = cameraPos;// - billboardPos;
	look /= look.length();

	// Create billboard right and forward vectors
	glm::vec3 right = glm::cross( up, look );
	// Do not rotate as camera moves backward and forward
	right.z = 0;

	glm::vec3 forward = glm::cross( up, right );
	forward /= forward.length();

	// angle between camera position and forward
	float dot = glm::dot( forward, look );

	// add angle between camera view and forward
	look = mCamera->GetViewDirection() - billboardPos;
	forward = glm::cross( up, right );
	dot += glm::dot( forward, look );

	mTransform.rotation.y = dot;
}
// ---------------------------------------------------------------------------