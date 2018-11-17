#include "EntityBat.h"
#include "j1Entity.h"
#include "j1ModuleEntity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "ModuleFadeToBack.h"
#include "j1Collision.h"
#include "j1Render.h"


EntityBat::EntityBat(fPoint pos,Animation* anim, SDL_Texture* tex):j1Entity(pos,tex)
{

	anim_bat.speed = anim->speed;
	for (int j = 0; j<anim->numFrames; ++j)
	{
		SDL_Rect frame = anim->ReturnFrame(j);
		anim_bat.PushBack(frame);
	}

	pugi::xml_node nodeBat = App->entity->entitiesNodeDoc.child("bat");
	pugi::xml_node nodeCol = nodeBat.child("collider");
	rectMesure = { nodeCol.attribute("w").as_int(), nodeCol.attribute("h").as_int() };

	SDL_Rect playerBat = { (pos.x - rectMesure.x / 2), (pos.y - rectMesure.y), rectMesure.x, rectMesure.y };
	collider = App->collision->AddCollider(playerBat, COLLIDER_PLAYER, App->entity);


}

EntityBat::~EntityBat()
{
}


bool EntityBat::PreUpdate(float dt)
{
	this->dt = dt;
	
	if (timer.ReadSec() > 1.0f)
	{
		iPoint origin = App->map->WorldToMap((int)position.x, (int)position.y- halfTileSize);

		iPoint p = App->map->WorldToMap(App->entity->entity_player->position.x, App->entity->entity_player->position.y - halfTileSize);

		int manhattan = App->pathfinding->ManhattanDistance(origin, p);

		if (manhattan < 20)
		{
			if (App->pathfinding->CreatePath(origin, p, FLYING)==1)
			{
				bat_path.Clear();
				const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
				for (int i = 0; i < path->Count(); i++)
				{
					bat_path.PushBack(*path->At(i));
				}
			}
		}
	}
	collider->SetPos((position.x + speed.x * dt) - collider->rect.w / 2, (position.y + speed.y * dt) - collider->rect.h);
	return true;
}








void EntityBat::Move(float dt)
{
	
	uint sizePath = 0;
	sizePath = bat_path.Count();

	if (sizePath > 0)
	{
		sizePath -= 1;
		iPoint batPos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);
		int i = 0;
		bool findPositionOnPath = false;
		for (i; i < sizePath; ++i)
		{
			if (*bat_path.At(i) == batPos)
			{
				findPositionOnPath = true;
				break;
			}
		}
		if (findPositionOnPath)
		{
			if (i < sizePath)
			{
				speed.x = bat_path[i + 1].x - bat_path[i].x;
				speed.y = bat_path[i + 1].y - bat_path[i].y;
			}
		}
	}

	else
	{
		speed = { 0,0 };

	}
	position.x += speed.x;
	position.y += speed.y;

	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);

}




void EntityBat::Draw()
{
	SDL_Rect frameAnim = anim_bat.GetCurrentFrame(dt);
	
		if (right)
			App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
		else
			App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
		
		for (uint i = 0; i < bat_path.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(bat_path.At(i)->x, bat_path.At(i)->y);
			App->render->Blit(App->pathfinding->debug_tex, pos.x, pos.y);
		}
}

void EntityBat::OnCollision(Collider * collider)
{
	/*if (collider->type == COLLIDER_PLAYER)*/
		
}




