#ifndef __J1ENTITY_H_
#define __J1ENTITY_H_

#include "j1Module.h"
#include "Animation.h"
#include "j1Collision.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;

/*enum Entity_State
{
	STATE_IDLE,
	STATE_WALK, 
	STATE_DEATH,
	STATE_ATTACK
};*/

class j1Entity :public j1Module
{
public:
	p2SString String_docXml;
	SDL_Texture* entity_texture = nullptr;
	pugi::xml_node entity_node;
	pugi::xml_document	entity_file;

	Animation LoadAnimations(p2SString name);

	// EntityState = Entity_State::STATE_IDLE;
	Animation EntityIdle;
	Animation EntityWalk;
	Animation EntityDeath;
	Animation EntityAttack;

	



public:
	j1Entity();
	bool Awake(pugi::xml_node&);
	void Init();
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


private:
	bool Draw();


};

#endif // __j1ENTITY_H__