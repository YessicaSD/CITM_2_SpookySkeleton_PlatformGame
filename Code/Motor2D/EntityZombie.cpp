#include "EntityZombie.h"
#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "p2Defs.h"
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

	collider = App->collision->AddCollider({(int)pos.x,(int)pos.y,nodeCol.attribute("w").as_int(),nodeCol.attribute("h").as_int() }, COLLIDER_ENTITY, App->entity);
	colAttack = App->collision->AddCollider({ (int)pos.x + 5,(int)pos.y,nodeCol.attribute("w").as_int(),nodeCol.attribute("h").as_int() }, COLLIDER_IGNORE_HIT, App->entity);;
	timer.Start();
	entityPlayerTarget = App->entity->entity_player;
	
}


bool EntityZombie::PreUpdate(float dt)
{
	this->dt = dt;
	moveDown = true;
	if (timer.ReadSec()>=1 && entityPlayerTarget!=nullptr)
	{
		playerPos = App->map->WorldToMap(entityPlayerTarget->position.x, entityPlayerTarget->position.y - halfTileSize);
		iPoint zombiePos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);

		int manhattan = App->pathfinding->ManhattanDistance(playerPos, zombiePos);
		if (manhattan < 10 
			&& App->pathfinding->CreatePath(zombiePos, playerPos,WALKING) == 1)
			{
				path.Clear();
				const p2DynArray<iPoint>* pathIter = App->pathfinding->GetLastPath();
				for (int i = 0; i < pathIter->Count(); ++i)
				{
					path.PushBack(*pathIter->At(i));
				}

			}
		
		timer.Start();
	}
	collider->SetPos((position.x + speed.x) - collider->rect.w *0.5F, (position.y + speed.y) - collider->rect.h);
	return true;
	
}

void EntityZombie::Move(float dt)
{
	
	
	uint sizePath = 0;
	sizePath=path.Count();
	
	if (sizePath > 0)
	{
		if(state == State_zomby::STATE_IDLE)
			state = State_zomby::STATE_WALK;
		sizePath -= 1;
 		iPoint zombiePos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);
		int i =0 ;
		bool findPositionOnPath = false;
		for (i;i<sizePath;++i)
		{
			if (*path.At(i) == zombiePos)
			{
				findPositionOnPath = true;
				break;
			}
		}
		if (findPositionOnPath)
		{
			if (i < sizePath)
			{
				speed.x = path[i + 1].x - path[i].x;
				right = (speed.x > 0) ? true : false;
			}
		}
		else
		{
			path.Clear();
			if (state!=State_zomby::STATE_ATTACK)
			{
				
				state = State_zomby::STATE_ATTACK;
			}
			
			

		}
			
		
	}
	else
	{
		if(state == State_zomby::STATE_WALK)
		state = State_zomby::STATE_IDLE;
		speed = {0,0 };

	}
	position.x += speed.x;
	position.y += speed.y;
	if (moveDown)
		speed.y += 1;

	if(right)
		colAttack->SetPos(position.x , position.y - collider->rect.h);
	else
	{
		colAttack->SetPos(position.x - colAttack->rect.w, position.y - collider->rect.h);
	}
	collider->SetPos(position.x - collider->rect.w * 0.5F, position.y - collider->rect.h);
}
void EntityZombie::OnCollision(Collider * otherColl)
{

	bool isOn = (int)position.y <= otherColl->rect.y && (int)(position.x) > otherColl->rect.x && (int)(position.x) < otherColl->rect.x + otherColl->rect.w;
	bool isOnTheLeft = position.x < otherColl->rect.x && (int)position.y > otherColl->rect.y;
	bool isOnTheRight = position.x > otherColl->rect.x + otherColl->rect.w && (int)position.y > otherColl->rect.y;
	bool isUnder = position.y > otherColl->rect.y + otherColl->rect.h && collider->rect.x + collider->rect.w - 5 > otherColl->rect.x && collider->rect.x + 5 < otherColl->rect.x + otherColl->rect.w;


	if (otherColl->type == COLLIDER_WALL || otherColl->type == COLLIDER_ICE || otherColl->type == COLLIDER_SPECIAL)
	{
		if (isOn)
		{
			moveDown = false;
			speed.y = (otherColl->rect.y - (int)position.y);
			}

		
		if (otherColl->type != COLLIDER_SPECIAL)
		{
			if (isOnTheLeft)
				speed.x = otherColl->rect.x - ((int)position.x + collider->rect.w *0.5F);

			if (isOnTheRight)
				speed.x = (otherColl->rect.x + otherColl->rect.w) - (position.x - collider->rect.w *0.5F);

			if (isUnder)
				speed.y = (otherColl->rect.y + otherColl->rect.h) - ((int)position.y - collider->rect.h);
		}

	}

}

void EntityZombie::Draw()
{
	SDL_Rect frameAnim = animation[(uint)state].GetCurrentFrame(dt);
	if (state== State_zomby::STATE_ATTACK)
	{
		if ((int)animation[(uint)state].current_frame == 8)
		{
			colAttack->type = COLLIDER_ENEMY;
		}
		if (animation[(uint)State_zomby::STATE_ATTACK].Finished())
		{
			colAttack->type = COLLIDER_IGNORE_HIT;
			state = State_zomby::STATE_IDLE;
			animation[(uint)State_zomby::STATE_ATTACK].Reset();
		}
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



