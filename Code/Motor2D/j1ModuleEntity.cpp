#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"


#include "EntityBat.h"
#include "EntityZombie.h"

ModuleEnemies::ModuleEnemies()
{
	name.create("enemies");
}

ModuleEnemies::~ModuleEnemies()
{

}


bool ModuleEnemies::PreUpdate(float dt)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE &&
				queue[i].x * App->win->scale < App->render->camera.x + (App->render->camera.w * App->win->scale) + 50)
				{
					SpawnEnemy(queue[i]);
					queue[i].type = ENEMY_TYPES::NO_TYPE;
					LOG("Spawning enemy at %d", queue[i].x * App->win->scale);
				}
		
	}*/

	return true;
}

bool ModuleEnemies::Update(float dt)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();
			
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);*/

	return true;
}

bool ModuleEnemies::PostUpdate()
{
	return true;
}

bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");
	/*App->tex->UnLoad(sprites);
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}*/
	return true;
}

void ModuleEnemies::OnCollision(Collider * c1, Collider * c2)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2);
			delete enemies[i];
			enemies[i] = nullptr;
			break;
		}
	}*/
}

j1Entity* ModuleEnemies::AddEnemy(ENEMY_TYPES type, fPoint pos)
{
	j1Entity* newEntity = nullptr;
	static_assert(UNKNOW >= 3, "code need update");
	switch (type)
	{
	case PLAYER:
		break;
		case ENEMY_BAT:
			newEntity = new EntityBat(pos);
		break;

		case ENEMI_ZOMBIE:
			newEntity = new EntityZombie(pos);
		break;
	}
		list_Entities.add(newEntity);

	return nullptr;
}

bool ModuleEnemies::DestroyEntity(j1Entity * entity)
{
	if (entity == nullptr)
		return false;

	bool found = false;

	p2List_item<j1Entity*>* itemEntity = list_Entities.start;	
	for (itemEntity; itemEntity != nullptr || found!= false; itemEntity = itemEntity->next)
	{
		if (itemEntity->data == entity)
			found = true;	
	}
	if (found)
	{
		list_Entities.del(itemEntity);
		return true;
	}
	return false;
}




void ModuleEnemies::SpawnEnemy(const EnemyInfo & info)
{
	// find room for the new enemy
	/*uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
			case ENEMY_TYPES::ENEMY_BAT:
			enemies[i] = new EntityBat(info.x, info.y);
			break;
		
		}
	}*/
}




