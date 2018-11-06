
#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "PugiXml\src\pugixml.hpp"
#include "j1Module.h"

#include "p2SString.h"
#include "p2Point.h"
#include "Animation.h"


class j1App;
 struct SDL_Texture;
 enum class AnimationState : uint
{
	ANIM_STATE_IDLE,
	ANIM_STATE_WALK,
	ANIM_STATE_JUMP,
	ANIM_STATE_ATTACK,
	ANIM_STATE_DEATH,
	ANIM_STATE_SPAWN
};
class j1Player : public j1Module
{
 public:
	
	fPoint flplayerPosSaved;
	fPoint flPos;
	fPoint flSpeed;
	fPoint offset;
	fPoint distansToCam;
	bool moveDown=true;
	bool canJump=true;
	bool attack=false;
	bool death_fx=false;
	bool jump_fx=false;
	bool debugMode = false;
	bool fading = false;
	bool loading=false;
	fPoint speed;
	


	AnimationState animState = AnimationState::ANIM_STATE_SPAWN;
	Animation PlayerIdle;
	Animation PlayerWalk;
	Animation PlayerJump;
	Animation PlayerAttack;
	Animation PlayerDeath;
	Animation PlayerSpawn;
	uint death_anim_fx, jump, death;

	pugi::xml_node player_node;
	SDL_Texture* ptexture=nullptr;



	p2SString String_docXml;
	Collider* ColliderPlayer=nullptr;
	

	Animation LoadAnimations(p2SString name);
	bool CreateCol();
	void DebugModeInput();

public:
	j1Player();
	void Init();
	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	bool PostUpdate();

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
	bool Draw();
	


};

#endif // __j1MODULE_H__
