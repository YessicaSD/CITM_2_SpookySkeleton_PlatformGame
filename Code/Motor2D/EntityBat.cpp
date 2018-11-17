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
	collider->SetPos((position.x + speed.x * dt) - collider->rect.w / 2, (position.y + speed.y * dt) - collider->rect.h);

	iPoint origin= App->map->WorldToMap(position.x, position.y);
	
	iPoint p = App->map->WorldToMap(App->entity->entity_player->position.x, App->entity->entity_player->position.y-16);

	int manhattan = App->pathfinding->ManhattanDistance(origin, p);
	
	if (manhattan < 15)
	{
		App->pathfinding->CreatePath(origin, p);
		const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
		for (int i = 0; i < path->Count(); i++)
		{
			bat_path.PushBack(*path->At(i));
		}
	}
	
	return true;
}








void EntityBat::Move(float dt)
{
	if (timer.ReadSec()>1.0f)
	{
		
		if (bat_path.Count() > 0)
		{
			
			ibat_pos = App->map->WorldToMap(position.x, position.y);
			/*if (i<bat_path.Count())
			{
				if (ibat_pos == bat_path[i + 1])
				{
					speed_bat.x = bat_path[i + 1].x - bat_path[i].x;
					speed_bat.y = bat_path[i + 1].y + bat_path[i].y;
					ibat_pos.x += speed.x;
					ibat_pos.y += speed.y;
					i++;
				}
			}*/
			collider->SetPos(ibat_pos.x - collider->rect.w / 2, ibat_pos.y - collider->rect.h);

		}

		timer.Start();
	}
}




void EntityBat::Draw()
{
	SDL_Rect frameAnim = anim_bat.GetCurrentFrame(dt);
	
		if (right)
			App->render->Blit(texture, ibat_pos.x - frameAnim.w / 2, ibat_pos.y - frameAnim.h, &frameAnim);
		else
			App->render->Blit(texture, ibat_pos.x - frameAnim.w / 2, ibat_pos.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
	
}

void EntityBat::OnCollision(Collider * collider)
{
	/*if (collider->type == COLLIDER_PLAYER)*/
		
}




