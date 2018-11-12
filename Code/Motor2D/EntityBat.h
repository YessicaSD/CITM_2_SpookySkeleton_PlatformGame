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
	// EntityState = Entity_State::STATE_IDLE;
	Animation Anim;
	Animation EntityIdle;
	Animation EntityWalk;
	Animation EntityDeath;
	Animation EntityAttack;

	fPoint Bat_Pos;



public:
	EntityBat(int x,int y);
	/*bool Awake(pugi::xml_node&) override;
	void Init();
	bool Start() override;

	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp()override;
	bool Draw(float dt) override;

	bool Load(pugi::xml_node&) override;
	bool Save(pugi::xml_node&) const override;*/

};
#endif // !ENTITYBAT_H_


