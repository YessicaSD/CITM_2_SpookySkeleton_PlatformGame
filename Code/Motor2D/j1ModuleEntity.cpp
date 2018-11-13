#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1map.h"
#include "j1Pathfinding.h"

ModuleEnemies::ModuleEnemies()
{
	name.create("enemies");
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}
bool ModuleEnemies::Awake(pugi::xml_node& config)
{
	return true;
}
bool ModuleEnemies::Start()
{
	sprites = App->tex->Load("textures/enemies.png");
	debug_tex = App->tex->Load("textures/pathfinding1.png");
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

	// Debug pathfinding ------------------------------
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
	}

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




