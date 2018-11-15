#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#include "Animation.h"
#define MAX_ENEMIES 50


enum ENEMY_TYPES
{
	PLAYER =0,
	ENEMY_BAT,
	ENEMI_ZOMBIE,
	UNKNOW,
};

class j1Entity;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::UNKNOW;
	int x, y;
};

class ModuleEnemies : public j1Module
{
private:
	p2List<j1Entity*> list_Entities;
	p2List<Animation*> entitiesAnimation;
	bool LoadAnimations(pugi::xml_node animNode);
public:
	pugi::xml_document	enemiesFile;
	pugi::xml_node enemiesNodeDoc;

	SDL_Texture* playerTexture=nullptr;
	SDL_Texture* entitiesTexture=nullptr;
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

	j1Entity* AddEntity(ENEMY_TYPES type, fPoint pos);
	bool DestroyEntity(j1Entity* entity);
	
private:
	float dt=0.0F;

};
#endif // __ModuleEnemies_H__

