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
private:
	Animation animation[STATE_MAX];
	fPoint maxSpeed = { 0.0F,0.0F };
	iPoint distansToCam = { 0,0 };
	p2SString str_coin;
	p2SString str_points;
	

	bool canJump = false;
	bool debugMode = false;
	bool right = true;
	bool iceMovement = false;
	bool death_fx = false;
	bool get_hurt = false;
	bool jump_fx = false;
	uint acceleration_x=100;
	void DebugModeInput();
public:
		Player(fPoint position,Animation* anim,SDL_Texture* tex, entities_types type);
		~Player();
		bool PreUpdate(float dt);
		void Move(float dt);
		void Draw() override;
		void OnCollision(Collider* collider);
		PlayerState state = STATE_SPAWN;
		

};

#endif // __PLAYER_H_
