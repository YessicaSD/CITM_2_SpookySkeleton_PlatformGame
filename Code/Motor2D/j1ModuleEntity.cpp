#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Entity.h"

ModuleEnemies::ModuleEnemies()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{
}

bool ModuleEnemies::Start()
{
	sprites = App->tex->Load("rtype/enemies.png");
	return true;
}

bool ModuleEnemies::PreUpdate(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE &&
				queue[i].x * App->win->scale < App->render->camera.x + (App->render->camera.w * App->win->scale) + 50)
				{
					SpawnEnemy(queue[i]);
					queue[i].type = ENEMY_TYPES::NO_TYPE;
					LOG("Spawning enemy at %d", queue[i].x * App->win->scale);
				}
		
	}

	return true;
}

bool ModuleEnemies::Update(float dt)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();
			
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);
	return true;
}

bool ModuleEnemies::PostUpdate()
{
	return true;
}

bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");
	App->tex->UnLoad(sprites);
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}
	return true;
}

void ModuleEnemies::OnCollision(Collider * c1, Collider * c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2);
			delete enemies[i];
			enemies[i] = nullptr;
			break;
		}
	}
}

bool ModuleEnemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}


void ModuleEnemies::SpawnEnemy(const EnemyInfo & info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		/*case ENEMY_TYPES::REDBIRD:
			enemies[i] = new Enemy_RedBird(info.x, info.y);
			break;*/
		
		}
	}
}




