
#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "PugiXml\src\pugixml.hpp"
#include "j1Module.h"

#include "p2SString.h"
#include "p2Point.h"
#include "Animation.h"


class j1App;
 struct SDL_Texture;
 enum PlayerState : uint
{
	STATE_IDLE,
	STATE_WALK,
	STATE_JUMP,
	STATE_ATTACK,
	STATE_DEATH,
	STATE_SPAWN
};
class j1Player : public j1Module
{

 public:
	 fPoint flPos = { 0.0F,0.0F };
	 fPoint speed = {0.0F,0.0F};
	 fPoint maxSpeed;
	 bool right = true;

	fPoint flplayerPosSaved;
	fPoint distansToCam;
	Collider* ColliderPlayer = nullptr;

	bool moveDown=true;
	bool canJump=true;
	bool attack=false;
	bool death_fx=false;
	bool jump_fx=false;
	bool debugMode = false;
	bool fading = false;
	bool loading=false;
	
	
	pugi::xml_document	player_file;
	pugi::xml_node player_node;

	PlayerState PlayerState = PlayerState::STATE_SPAWN;
	Animation PlayerIdle;
	Animation PlayerWalk;
	Animation PlayerJump;
	Animation PlayerAttack;
	Animation PlayerDeath;
	Animation PlayerSpawn;

	uint death_anim_fx, jump, death;

	SDL_Texture* ptexture=nullptr;



	Animation LoadAnimations(p2SString name);
	bool CreateCol();
	void DebugModeInput();

public:
	j1Player();
	void Init();
	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate(float dt) override;
	void OnCollision(Collider* c1, Collider* c2) override;
	bool Update(float dt) override;
	bool CleanUp();
	bool Draw(float dt) override;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	inline void SetPosPlayer_x(float x)
	{
		if (!fading)
		{
			flPos.x = x;

		}
	}
	inline void SetPosPlayer_y(float y)
	{
		if (!fading)
		{
			flPos.y = y;
		}
	}
	
	


};

#endif // __j1MODULE_H__
