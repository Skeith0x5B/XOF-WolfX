#include "WolfX_GameObjects.hpp"
#include "WolfX_Events.hpp"
#include "WolfX_ResourcePaths.hpp"


// PLAYER
Player::Player( EngineSubSystems *engineSystems, glm::vec3 spawnPos, 
	Mesh *mesh, Sprite *sprite, Material *material, FirstPersonCamera *camera ) :
	GameObject( engineSystems, spawnPos, mesh, sprite, material, camera ) {
	mState = NEUTRAL;
}

Player::~Player() {
}

void Player::Update( float dt ) {
	//Renderer->Draw( *this ); // MOVE TO REQUEST + BATCH SYSTEM

	// TODO: FIX - Give sprites their own animation rate to go along with dt
	switch( mState ) {
		case FIRING:
			if( mSprite->GetCurrentFrame() >= ( mSprite->GetAnimationFrameCount() - 1 ) ) {
				mSprite->ResetAnimation();
				mState = NEUTRAL;
			} else {
				mSprite->StepAnimation( dt * 0.01f );
			}
		break;
	}

	// SUBMIT RENDERING REQUEST
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
		case WOLFX_EVENTS::PLAYER_PRESSED_FIRE_KEY:
			mState = FIRING;
			break;
	}
}
// ---------------------------------------------------------------------------


// BILLBOARD
BillBoard::BillBoard( EngineSubSystems *engineSystems, glm::vec3 spawnPos, 
	Mesh *mesh, Sprite *sprite, Material *material, FirstPersonCamera *camera ) :
	GameObject( engineSystems, spawnPos, mesh, sprite, material, camera ) {
}

BillBoard::~BillBoard() {
}

void BillBoard::Update( float dt ) {
	mEngine->renderer->SubmitRenderRequest( RenderRequest( 
		XOF_RENDER_REQUEST_TYPE::RENDER_SPRITE, mMaterial, nullptr, mSprite,
		mCamera, &mTransform ) );
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
Mover::Mover( EngineSubSystems *engineSystems, MOVER_OBJECT_AXIS directionToMove, bool isTwoWay, 
	float amountToMove, I8 movementStepSign, float movementSpeed, 
	glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, Material *material, FirstPersonCamera *camera) : 
	GameObject( engineSystems, spawnPos, mesh, sprite, material, camera ) { 
	mDirectionToMoveAlong = directionToMove;
	mStartingPos = spawnPos;
	mState = MOVER_OBJECT_STATES::CLOSED;
	mMovementSpeed = movementSpeed;
	mAmountToMoveBy = amountToMove;
	mAmountMoved = 0.f;
	mMovementStepSign = movementStepSign;
	mIsTwoWay = isTwoWay;
}

Mover::~Mover() {
}

void Mover::Update( float dt ) {
	float absMoved = 0.f, absMoveBy = 0.f;

	if( mState == OPENING || mState == CLOSING ) {
		absMoved = std::abs( mAmountMoved );
		absMoveBy = std::abs( mAmountToMoveBy );
		switch( mState ) {
			case OPENING:	if( absMoved > absMoveBy ) {
								mState = OPEN;
								mAmountMoved = 0.f;
								if( !mIsTwoWay ) {
									mState = ONE_WAY_FINISHED;
								}
							} else {
								float step = mMovementStepSign * ( mMovementSpeed * dt );
								mAmountMoved += step;
								mTransform.translation[mDirectionToMoveAlong] += step;
							}
							break;

			case CLOSING:	if( absMoved > absMoveBy ) {
								mState = CLOSED;
								mAmountMoved = 0.f;
							} else {
								float step = -( mMovementStepSign * ( mMovementSpeed * dt ) );
								mAmountMoved += step;
								mTransform.translation[mDirectionToMoveAlong] += step;
							}
							break;
		}
	}

	mEngine->renderer->SubmitRenderRequest( RenderRequest( 
		XOF_RENDER_REQUEST_TYPE::RENDER_MESH, mMaterial, mMesh, nullptr,
		mCamera, &mTransform ) );
}

static float WOLFX_MOVER_MIN_DIST_FROM_PLAYER = 1.3f;
void Mover::OnEvent( const Event *_event ) {
	switch( _event->type ) {
		case WOLFX_EVENTS::PLAYER_SPAWNED: break;
		case WOLFX_EVENTS::PLAYER_MOVED: break;
		case WOLFX_EVENTS::PLAYER_PRESSED_ACTION_KEY: 
			// NOTE: GLM length() function not working as expected here?
			glm::vec3 dist = mTransform.translation - 
				glm::vec3 ( _event->arguments[0].asVec3f[0], 0.f, _event->arguments[0].asVec3f[2] );
			// Skip srtf as it's expensive?
			float l = std::sqrtf( (dist.x * dist.x) + (dist.z * dist.z) );
			if( l <= WOLFX_MOVER_MIN_DIST_FROM_PLAYER ) {
				Move();
			}
			break;
	}
}

void Mover::SetMovementDirection( Mover::MOVER_OBJECT_AXIS direction ) {
	mDirectionToMoveAlong = direction;
}

void Mover::SetMovementDistance( float distance ) {
	mAmountToMoveBy = distance;
}

void Mover::SetMovementSpeed( float speed ) {
	mMovementSpeed = speed;
}

void Mover::SetMovementStepSign( I8 sign ) {
	mMovementStepSign = sign;
}

void Mover::Move() {
	switch( mState ) {
		case OPEN: mState = CLOSING; break;
		case OPENING: // fallthrough to play sound
		case CLOSING: mEngine->audio->SubmitAudioRequest( AudioRequest(
						XOF_AUDIO_REQUEST_TYPE::PLAY_SOUND_EFFECT, nullptr, nullptr,
						50.f, WOLFX_RESOURCE::AUDIO + "DOOROPENANDCLOSE.wav" ) ); break;
		case CLOSED: mState = OPENING; break;
	}
}
// ---------------------------------------------------------------------------


// ENEMY
Enemy::Enemy( EngineSubSystems *engineSystems, glm::vec3 spawnPos, Mesh *mesh, Sprite *sprite, 
	Material *material, FirstPersonCamera *camera) : GameObject( engineSystems, spawnPos, mesh, sprite, material, camera ) { 
}

Enemy::~Enemy() {
}

void Enemy::Update( float dt ) {
	mSprite->StepAnimation( 0.001f );
	mEngine->renderer->SubmitRenderRequest( RenderRequest( 
		XOF_RENDER_REQUEST_TYPE::RENDER_SPRITE, mMaterial, nullptr, mSprite,
		mCamera, &mTransform ) );
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