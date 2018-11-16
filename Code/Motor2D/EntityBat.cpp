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
	
	App->pathfinding->CreatePath(origin, p);
	return true;
}








void EntityBat::Move(float dt)
{
	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);
	/*iPoint ibat_pos(position.x, position.y);
	iPoint iplayer_pos= App->render->ScreenToWorld(App->entity->entity_player->position.x, App->entity->entity_player->position.y);
	App->pathfinding->CreatePath(ibat_pos, iplayer_pos);
	bat_path = App->pathfinding->GetLastPath();
	
	for (int i=0;i<bat_path->Count();i++)
	{
		position.x = bat_path->At(i)->x;
		position.y = bat_path->At(i)->y;
	}*/
	iPoint ibat_pos(position.x, position.y);
	
	iPoint p = App->render->ScreenToWorld(position.x, position.y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);
	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		ibat_pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		
	}
}




void EntityBat::Draw()
{
	SDL_Rect frameAnim = anim_bat.GetCurrentFrame(dt);
	
	if (right)
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
	else
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
	
}

void EntityBat::OnCollision(Collider * collider)
{
	/*if (collider->type == COLLIDER_PLAYER)*/
		
}




