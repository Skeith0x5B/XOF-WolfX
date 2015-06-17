#ifndef WOLFX_GAME_OBJECTS_HPP
#define WOLFX_GAME_OBJECTS_HPP


#include "../GameplayFoundations/XOF_GameObject.hpp"


class Player : public GameObject {
public:
	enum PLAYER_OBJECT_STATES {
		NEUTRAL = 0,
		FIRING,
	};

							Player( EngineSubSystems *engineSystems, 
								glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
								Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
								Material *material = nullptr, FirstPersonCamera *camera = nullptr );
							~Player();
							
	virtual void			Update( float dt ) override;
	virtual void			OnEvent( const Event *_event ) override;

private:
	U8						mHealth;
	PLAYER_OBJECT_STATES	mState;
};
// ---------------------------------------------------------------------------


class BillBoard : public GameObject {
public:
					BillBoard( EngineSubSystems *engineSystems,
						glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
					~BillBoard();

	virtual void	Update( float dt ) override;
	virtual void	OnEvent( const Event *_event ) override;

private:
	void			FacePlayer( const glm::vec3& cameraPos );
};
// ---------------------------------------------------------------------------


class Mover : public GameObject {
public:
	enum MOVER_OBJECT_STATES {
		OPEN = 0,
		OPENING,
		CLOSING,
		CLOSED,
		ONE_WAY_FINISHED,
	};

	enum MOVER_OBJECT_AXIS {
		X = 0,
		Y,
		Z,
	};

						Mover( EngineSubSystems *engineSystems, 
							MOVER_OBJECT_AXIS directionToMove, bool isTwoWay = true, 
							float amountToMove = 1.f, I8 movementStepSign = 1, float movementSpeed = 0.005f,
							glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
							Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
							Material *material = nullptr, FirstPersonCamera *camera = nullptr );
						~Mover();

	virtual void		Update( float dt ) override;
	virtual void		OnEvent( const Event *_event ) override;

	void				SetMovementDirection( MOVER_OBJECT_AXIS direction );
	void				SetMovementDistance( float distance );
	void				SetMovementSpeed( float speed );
						// Specifies wether we move back or forward with each step
	void				SetMovementStepSign( I8 sign );
	void				SetTwoWay( bool isTwoWay );

private:
	MOVER_OBJECT_AXIS	mDirectionToMoveAlong;
	float				mAmountToMoveBy;
	float				mAmountMoved;
	float				mMovementSpeed;
	glm::vec3			mStartingPos;
	U8					mState;
	I8					mMovementStepSign;
	bool				mIsTwoWay;

	void				Move();
};
// ---------------------------------------------------------------------------


class Enemy : public GameObject {
public:
					Enemy( EngineSubSystems *engineSystems,
						glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
					~Enemy();

	virtual void	Update( float dt ) override;
	virtual void	OnEvent( const Event *_event ) override;

private:
	U32				mHealth;
	void			FacePlayer( const glm::vec3& cameraPos );
};
// ---------------------------------------------------------------------------


/*
class Pickup : public GameObject {
public:
					Pickup( U32 value, glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
					~Pickup();

	virtual void	Update( float dt ) override;
	virtual void	OnEvent( Event *_event ) override;

private:
	U32				mValue;
};
typedef Pickup HealthPack, Turkey, Ammo;
// ---------------------------------------------------------------------------
*/


#endif // WOLFX_GAME_OBJECTS_HPP