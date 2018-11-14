#ifndef ENTITYBAT_H_
#define ENTITYBAT_H_
#include "j1Entity.h"
#include "p2Point.h"

class EntityBat : public j1Entity
{
public:
	p2SString String_docXml;
	SDL_Texture* entity_texture = nullptr;
	pugi::xml_node entity_node;
	pugi::xml_document	entity_file;

	Animation LoadAnimations(p2SString name);
	
	Animation Anim;
	





public:
	EntityBat(int x,int y);
	bool Draw(float dt);
	bool PreUpdate(float dt) ;
	void Move();
	bool CleanUp();
	
};
#endif // !ENTITYBAT_H_


