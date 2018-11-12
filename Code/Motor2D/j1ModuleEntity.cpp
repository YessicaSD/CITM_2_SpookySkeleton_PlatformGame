#include "j1ModuleEntity.h"

ModuleEnemies::ModuleEnemies()
{
}

ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{
	return true;
}

bool ModuleEnemies::CleanUp()
{
	return true;
}

void ModuleEnemies::OnCollision(Collider * c1, Collider * c2)
{
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	return true;
}

Enemy * ModuleEnemies::InstaSpawn(ENEMY_TYPES type, int x, int y)
{
	return nullptr;
}

void ModuleEnemies::ClearQueues()
{
}

Enemy * ModuleEnemies::SpawnEnemy(const EnemyInfo & info)
{
	return nullptr;
}

void ModuleEnemies::RenderEnemy(Enemy *, int layer)
{
}


