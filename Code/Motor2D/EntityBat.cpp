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
	}
	
	return true;
}








void EntityBat::Move(float dt)
{
	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);
	

	
	ibat_pos = App->map->WorldToMap(position.x, position.y);
	ibat_pos = App->map->MapToWorld(ibat_pos.x, ibat_pos.y);
	
	
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	
	for (uint i = 0; i < path->Count(); ++i)
	{
		ibat_pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		ibat_pointer.add(ibat_pos);
		
	}
}




void EntityBat::Draw()
{
	SDL_Rect frameAnim = anim_bat.GetCurrentFrame(dt);
	p2List_item<iPoint>* item_pos = ibat_pointer.start;
	for (item_pos;item_pos!=nullptr;item_pos=item_pos->next)
	{
		if (right)
			App->render->Blit(texture, item_pos->data.x - frameAnim.w / 2, item_pos->data.y - frameAnim.h, &frameAnim);
		else
			App->render->Blit(texture, item_pos->data.x - frameAnim.w / 2, item_pos->data.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
	}
}

void EntityBat::OnCollision(Collider * collider)
{
	/*if (collider->type == COLLIDER_PLAYER)*/
		
}




