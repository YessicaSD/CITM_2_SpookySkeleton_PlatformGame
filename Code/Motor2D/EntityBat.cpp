#include "EntityBat.h"
#include "j1Entity.h"
#include "j1ModuleEntity.h"
#include "Player.h"
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
#include "Brofiler/Brofiler.h"


EntityBat::EntityBat(fPoint pos,Animation* anim, SDL_Texture* tex, entities_types type):j1Entity(pos,tex, type)
{

	for (uint AnimNum = 0; AnimNum < (uint)BatState::STATE_MAX; ++AnimNum)
	{
		anim_bat[AnimNum].speed = anim[AnimNum].speed;
		for (int j = 0; j < anim[AnimNum].numFrames; ++j)
		{
			anim_bat[AnimNum].PushBack(anim[AnimNum].ReturnFrame(j));
		}
	}
	
	pugi::xml_node nodeBat = App->entity->entitiesNodeDoc.child("bat");
	pugi::xml_node nodeCol = nodeBat.child("collider");
	rectMesure = { nodeCol.attribute("w").as_int(), nodeCol.attribute("h").as_int() };

	SDL_Rect playerBat = { (pos.x - rectMesure.x / 2), (pos.y - rectMesure.y), rectMesure.x, rectMesure.y };
	collider = App->collision->AddCollider(playerBat, COLLIDER_ENTITY, App->entity);
	
	batdeath_fx = true;
}

EntityBat::~EntityBat()
{
}


bool EntityBat::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_EntityBat.cpp", Profiler::Color::Salmon)
	this->dt = dt;
	
	if (state == BatState::STATE_DEATH && batdeath_fx)
		{
			App->audio->PlayFx(App->entity->fx_batdeath);
			batdeath_fx = false;
		}
	
	if (timer.ReadSec() > 1.0F)
	{
		iPoint origin = App->map->WorldToMap((int)position.x, (int)position.y- halfTileSize);

		iPoint p = App->map->WorldToMap(App->entity->entity_player->position.x, App->entity->entity_player->position.y - halfTileSize);

		int manhattan = origin.DistanceManhattan(p); 

		if (manhattan < 10 && App->pathfinding->CreatePath(origin, p, FLYING)==1)
			{
				bat_path.Clear();
				const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
				for (int pathIpoint = 0; pathIpoint < path->Count(); pathIpoint++)
				{
					bat_path.PushBack(*path->At(pathIpoint));
				}
			}
	}
	collider->SetPos((position.x + speed.x * dt) - collider->rect.w / 2, (position.y + speed.y * dt) - collider->rect.h);
	return true;
}








void EntityBat::Move(float dt)
{
	BROFILER_CATEGORY("Move_EntityBat.cpp", Profiler::Color::Black)
	uint sizePath = 0;
	sizePath = bat_path.Count();

	if (sizePath > 0)
	{
		sizePath -= 1;
		iPoint batPos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);
		int aux_i = 0;
		bool findPositionOnPath = false;
		for (aux_i; aux_i < sizePath; ++aux_i)
		{
			if (*bat_path.At(aux_i) == batPos)
			{
				findPositionOnPath = true;
				break;
			}
		}
		if (findPositionOnPath && aux_i < sizePath)
		{
				speed.x = bat_path[aux_i + 1].x - bat_path[aux_i].x;
				speed.y = bat_path[aux_i + 1].y - bat_path[aux_i].y;
		}
		
	}

	else
	{
		speed = { 0,0 };

	}
	position.x += speed.x*dt*40;
	position.y += speed.y*dt*40;
	if (speed.x > 0)
	{
		left = false;
	}
	else
	{
		left = true;
	}

	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);

}




void EntityBat::Draw()
{
	BROFILER_CATEGORY("Draw_EntityBat.cpp", Profiler::Color::AliceBlue)
	
	
	if (state == BatState::STATE_DEATH && anim_bat[(uint)BatState::STATE_DEATH].Finished())
	{
	
		toDelete = true;
	}

	SDL_Rect frameAnim = anim_bat[(uint)state].GetCurrentFrame(dt);

		if (left)
			App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
		else
			App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
		
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
			showPath = !showPath;

		if (showPath)
		{
			for (uint i = 0; i < bat_path.Count(); ++i)
			{
				iPoint pos = App->map->MapToWorld(bat_path.At(i)->x, bat_path.At(i)->y);
				App->render->Blit(App->pathfinding->debug_tex, pos.x, pos.y);
			}
		}

}

void EntityBat::OnCollision(Collider * otherCollider)
{
	
	if (otherCollider->type == COLLIDER_PLAYER)
	{
		bool PlayerIsOn = otherCollider->rect.y + otherCollider->rect.h*0.5F < collider->rect.y ;
		if (PlayerIsOn)
		{
			EntityBat::state = BatState::STATE_DEATH;
			collider->to_delete = true;
		}
		else
		{
			otherCollider->to_delete = true;
		}
	}
	
}




