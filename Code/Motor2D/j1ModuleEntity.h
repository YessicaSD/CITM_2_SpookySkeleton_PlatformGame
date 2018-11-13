#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
#include "p2Defs.h"
#define MAX_ENEMIES 50


enum ENEMY_TYPES
{
	NO_TYPE = -1,
	ENEMY_BAT,
	MAX_ENEMY
};

class j1Entity;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class ModuleEnemies : public j1Module
{
public:
	SDL_Texture * sprites = nullptr;
	pugi::xml_document	enemiesFile;
	pugi::xml_node enemiesNodeDoc;

public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Awake(pugi::xml_node& config)override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	void OnCollision(Collider* c1, Collider* c2) override;

	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	

private:
	void SpawnEnemy(const EnemyInfo& info);
	

private:
	EnemyInfo queue[MAX_ENEMIES];
	j1Entity* enemies[MAX_ENEMIES];
	
};

#endif // __ModuleEnemies_H__

