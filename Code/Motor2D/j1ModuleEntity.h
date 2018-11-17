#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "Animation.h"
#include "j1Entity.h"




class j1Entity;

struct EnemyInfo
{
	entities_types type = entities_types::UNKNOW;
	int x, y;
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

	j1Entity* AddEntity(entities_types type, fPoint pos);
	bool DestroyEntity(j1Entity* entity);
	void DestroyAllEntities();
private:
	float dt=0.0F;

};
#endif // __ModuleEnemies_H__

