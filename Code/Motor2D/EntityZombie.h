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

	/*Animation LoadAnimations(p2SString name);*/
	
	Animation EntityIdle;
	Animation EntityWalk;
	Animation EntityDeath;
	Animation EntityAttack;



public:
	EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex);
	bool Draw(float dt);
	
	bool PreUpdate(float dt);
	void Move();
	bool CleanUp();
};
#endif // !ENTITYZOMBIE_H_
