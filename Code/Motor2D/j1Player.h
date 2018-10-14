
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
	
	
	fPoint flPos;
	fPoint offset;
	fPoint distansToCam;
	bool moveDown;
	bool jumping;
	bool death_fx;
	bool jump_fx;
	bool debugMode = false;
	fPoint Speed;
	


	AnimationState animState = AnimationState::ANIM_STATE_SPAWN;
	Animation PlayerIdle;
	Animation PlayerWalk;
	Animation PlayerJump;
	Animation PlayerAttack;
	Animation PlayerDeath;
	Animation PlayerSpawn;
	uint death, jump;

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
	void SpawnPlayer();
	

	bool Draw();
	/*bool Load(pugi::xml_node&);*/
	//bool Save(pugi::xml_node&) const;

};

#endif // __j1MODULE_H__
