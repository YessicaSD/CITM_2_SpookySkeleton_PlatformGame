#ifndef __j1ModuleEnemies_H__
#define __j1ModuleEnemies_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Textures.h"
typedef unsigned __int32 uint32;
#define MAX_ENEMIES 50

struct Sfx;
enum ENEMY_TYPES
{
	NO_TYPE = -1,
	MAX_ENEMY
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
	fPoint speed;
	uint32 delay;//In miliseconds
	bool counting = false;
	uint32 spawnTime;
	Enemy *pointer = nullptr;
};

class ModuleEnemies : public j1Module
{

public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Start() override;

	bool CleanUp() override;
	void OnCollision(Collider* c1, Collider* c2) override;

	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	Enemy* InstaSpawn(ENEMY_TYPES type, int x, int y);

	void ClearQueues();

private:
	Enemy * SpawnEnemy(const EnemyInfo& info);
	void RenderEnemy(Enemy*, int layer);

private:
	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];

public:
	SDL_Texture* entitysTex = nullptr;
	

};

#endif // __ModuleEnemies_H__

