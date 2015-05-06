#ifndef WOLFX_GAME_OBJECTS_HPP
#define WOLFX_GAME_OBJECTS_HPP


#include "../GameplayFoundations/XOF_GameObject.hpp"


class Player : public GameObject {
public:
					Player( glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
					~Player();

	virtual void	Update( float dt ) override;
	virtual void	OnEvent( const Event *_event ) override;

private:
	U32				mHealth;
};
// ---------------------------------------------------------------------------


class BillBoard : public GameObject {
public:
					BillBoard( glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
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
	};

					Mover( const glm::vec3& directionToMove,
						glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
						Mesh *mesh = nullptr, Sprite *sprite = nullptr, 
						Material *material = nullptr, FirstPersonCamera *camera = nullptr );
					~Mover();

	virtual void	Update( float dt ) override;
	virtual void	OnEvent( const Event *_event ) override;

	void			SetMovementDirection( const glm::vec3& direction );

private:
	glm::vec3		mDirectionToMoveAlong;
	glm::vec3		mStartingPos;
	float			mMovementSpeed;
	U8				mState;
	

	void			Move();
};
// ---------------------------------------------------------------------------


class Enemy : public GameObject {
public:
					Enemy( 	glm::vec3 spawnPos = glm::vec3( 0.f, 0.f, 0.f ),
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