#ifndef ENTITYZOMBIE_H_
#define ENTITYZOMBIE_H_
#include "j1Entity.h"
#include "p2Point.h"

class EntityZombie : public j1Entity
{
public:
	p2SString String_docXml;
	SDL_Texture* entity_texture = nullptr;
	pugi::xml_node entity_node;
	pugi::xml_document	entity_file;

	Animation LoadAnimations(p2SString name);
	// EntityState = Entity_State::STATE_IDLE;
	Animation Anim;
	Animation EntityIdle;
	Animation EntityWalk;
	Animation EntityDeath;
	Animation EntityAttack;

	fPoint Zombie_Pos;



public:
	EntityZombie(int x, int y);
	
};
#endif // !ENTITYZOMBIE_H_
