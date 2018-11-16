#include "EntityZombie.h"
#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Pathfinding.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"

#include "j1Audio.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "ModuleFadeToBack.h"
#include "j1Collision.h"
#include "j1Render.h"


EntityZombie::EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex): j1Entity(pos,tex)
{
	for (uint i = 0; i < (uint)State_zomby::STATE_MAX; ++i)
	{
		animation[i].speed = anim[i].speed;
		for (int j = 0; j<anim[i].numFrames; ++j)
		{
			animation[i].PushBack(anim[i].ReturnFrame(j));
		}
	}
	pugi::xml_node nodeZombie = App->entity->entitiesNodeDoc.child("zombie");
	pugi::xml_node nodeCol = nodeZombie.child("collider");

	collider = App->collision->AddCollider({(int)pos.x,(int)pos.y,nodeCol.attribute("w").as_int(),nodeCol.attribute("h").as_int() }, COLLIDER_ENEMY, App->entity);
}


bool EntityZombie::PreUpdate(float dt)
{
	this->dt = dt;
	playerPos = App->map->WorldToMap(App->entity->entity_player->position.x, App->entity->entity_player->position.y- halfTileSize);
	iPoint zombiePos=App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);
	if (App->pathfinding->CreatePath(zombiePos, playerPos)==1)
	{
		path.Clear();
		const p2DynArray<iPoint>* pathIter = App->pathfinding->GetLastPath();
		for (int i = 0; i < pathIter->Count(); ++i)
		{
			path.PushBack(*pathIter->At(i));
		}
		
		
	}

	return true;
	
}

void EntityZombie::Move(float dt)
{
	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);
}

void EntityZombie::Draw()
{
	SDL_Rect frameAnim = animation[(uint)state].GetCurrentFrame(dt);
	if (animation[(uint)State_zomby::STATE_ATTACK].Finished())
	{
		state = State_zomby::STATE_IDLE;
		animation[(uint)State_zomby::STATE_ATTACK].Reset();
	}

	if (right)
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
	else
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
	int num = path.Count();

	for (uint i = 0; i < path.Count(); ++i)
	{

		iPoint pos = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
		App->render->Blit(App->pathfinding->debug_tex, pos.x, pos.y);
	}
}

bool EntityZombie::CleanUp()
{
	return true;
}



