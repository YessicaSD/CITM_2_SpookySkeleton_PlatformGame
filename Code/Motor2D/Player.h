#ifndef __PLAYER_H_
#define __PLAYER_H_

#include "j1Entity.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
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
	//PlayerState state = STATE_SPAWN;
	fPoint maxSpeed = { 0.0F,0.0F };
	iPoint distansToCam = { 0,0 };

	bool canJump = false;
	bool debugMode = false;
	bool right = true;
	bool iceMovement = false;
	bool death_fx = false;
	bool jump_fx = false;
	uint acceleration_x=200;
	void DebugModeInput();
public:
		Player(fPoint position,Animation* anim,SDL_Texture* tex, entities_types type);
		~Player();
		bool PreUpdate(float dt);
		void Move(float dt);
		void Draw() override;
		void OnCollision(Collider* collider);
		PlayerState state = STATE_SPAWN;
//protected:
//	Animation * animation = nullptr;
//	fPoint maxSpeed = { 0.0F,0.0F };
//
//	bool right = true;
//	float dt = 0.0F;
//	bool iceMovement = false;
//
//	fPoint flplayerPosSaved;
//	fPoint distansToCam;
//	Collider* ColliderPlayer = nullptr;
//
//	bool moveDown = true;
//	bool canJump = true;
//	bool attack = false;
//	bool death_fx = false;
//	bool jump_fx = false;
//	bool debugMode = false;
//	bool fading = false;
//	bool loading = false;
//
//
//	pugi::xml_document	player_file;
//	pugi::xml_node player_node;
//
//	PlayerState PlayerState = PlayerState::STATE_SPAWN;
//	Animation Player_State[STATE_MAX];
//
//	uint death_anim_fx, jump, death;
//	SDL_Texture* ptexture = nullptr;
//
//	Animation LoadAnimations(p2SString name);
//	bool CreateCol();
//	void DebugModeInput();
//
//

};

#endif // __PLAYER_H_
