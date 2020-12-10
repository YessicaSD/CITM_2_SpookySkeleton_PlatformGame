#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "j1Entity.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"


struct Collider;

enum PlayerState : uint
{
	STATE_IDLE,
	STATE_WALK,
	STATE_JUMP,
	STATE_ATTACK,
	STATE_DEATH,
	STATE_SPAWN,
	STATE_MAX
};
class Player: public j1Entity
{
public:
	Player(fPoint position, Animation* anim, SDL_Texture* tex, entities_types type);
	bool PreUpdate(float dt);
	void Move(float dt);
	void Draw() override;
	void OnCollision(Collider* collider);
	 void OnCollisionGround(Collider* collider) override;
	PlayerState state = STATE_SPAWN;

private:
	Animation animation[STATE_MAX];
	fPoint maxSpeed = { 0.0F,0.0F };
	float iceSpeed = 200;
	iPoint distansToCam = { 0,0 };
	p2SString str_coin;
	p2SString str_points;
	float jumpInitialVelocity = 0;
	float gravity;
	float maxHeight;
	float jumpDuration;
	float groundHeight = 10;

	bool isGrounded = false;
	bool onIce = false;
	bool debugMode = false;
	bool right = true;
	bool get_hurt = false;
	
	uint acceleration_x=100;
	

	void DebugModeInput();
	void HandleGroundCollider(Collider* col);
	void HandleBodyCollider(Collider* col);

};

#endif // __PLAYER_H_
