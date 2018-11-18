#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Entity.h"




class j1Entity;
class Player;

struct EntitiesInfo
{
	entities_types type = UNKNOW;
	fPoint pos = { 0.0F,0.0F };
	EntitiesInfo(entities_types type, fPoint pos) :type(type), pos(pos) {}
	EntitiesInfo() {}
};


class ModuleEnemies : public j1Module
{
private:
	p2List<Animation*> entitiesAnimation;
	bool LoadAnimations(pugi::xml_node animNode);
public:
	p2List<j1Entity*> list_Entities;
	pugi::xml_document	enemiesFile;
	pugi::xml_node entitiesNodeDoc;
	SDL_Texture* playerTexture=nullptr;
	SDL_Texture* entitiesTexture=nullptr;

	j1Entity* entity_player = nullptr;
	Player* player = nullptr;
public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Awake(pugi::xml_node&)override;
	bool Start()override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;
	void OnCollision(Collider* c1, Collider* c2) override;

	j1Entity* AddEntity(const EntitiesInfo& entity);
	bool DestroyEntity(p2List_item<j1Entity*>* entity);
	void DestroyAllEntities();
	uint fx_death;
	uint fx_jump;
	uint fx_batdeath;
private:
	float dt=0.0F;

};
#endif // __ModuleEnemies_H__

