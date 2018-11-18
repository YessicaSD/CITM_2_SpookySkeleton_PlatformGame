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

#include "j1Render.h"

#include "Brofiler/Brofiler.h"


EntityZombie::EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex, entities_types type): j1Entity(pos,tex, type)
{
	BROFILER_CATEGORY("EntityZombieConstructor.cpp", Profiler::Color::Salmon)
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
	BROFILER_CATEGORY("PreUpdate_EntityZombie.cpp", Profiler::Color::Salmon)
	this->dt = dt;
	if ((collider->to_delete == true || collider == nullptr) && state != State_zomby::STATE_DEATH)
		{
			state = State_zomby::STATE_DEATH;
			speed.x = 0.0F;
			colAttack->type = COLLIDER_IGNORE_HIT;
		}
		
	

	moveDown = true;
	if (state != State_zomby::STATE_ATTACK && timer.ReadSec() >= 1 && entityPlayerTarget != nullptr)
		{
			playerPos = App->map->WorldToMap(entityPlayerTarget->position.x, entityPlayerTarget->position.y - halfTileSize);
			iPoint zombiePos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);

			int manhattan = playerPos.DistanceManhattan(zombiePos);
			if (manhattan < 10
				&& App->pathfinding->CreatePath(zombiePos, playerPos, WALKING) == 1)
			{
				pathIndex = 0;
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
	BROFILER_CATEGORY("Move_EntityZombie.cpp", Profiler::Color::Black)
	
		if (state != State_zomby::STATE_DEATH)
		{
			uint sizePath = 0;
			sizePath = path.Count();

			if (sizePath > 0)
			{
				if (state == State_zomby::STATE_IDLE)
					state = State_zomby::STATE_WALK;
				iPoint zombiePos = App->map->WorldToMap((int)position.x, (int)position.y - halfTileSize);
				int i = 0;
				bool findPositionOnPath = false;
				for (i; i<sizePath; ++i)
				{
					if (*path.At(i) == zombiePos)
					{
						findPositionOnPath = true;
						break;
					}
				}
				if (findPositionOnPath)
				{
					if (i < sizePath - 1)
					{
						speed.x = path[i + 1].x - path[i].x;
						right = (speed.x > 0) ? true : false;
					}
					else
					{
						path.Clear();
						if (state != State_zomby::STATE_ATTACK)
						{

							state = State_zomby::STATE_ATTACK;
						}
					}
				}

			}
			else
			{
				if (state == State_zomby::STATE_WALK)
					state = State_zomby::STATE_IDLE;
				speed = { 0,0 };

			}

			position.x += speed.x * 50 * dt;
			position.y += speed.y * 20 * dt;
			if (moveDown)
				speed.y += 1 * 10 * dt;

			if (right)
				colAttack->SetPos(position.x, position.y - collider->rect.h);
			else
			{
				colAttack->SetPos(position.x - colAttack->rect.w, position.y - collider->rect.h);
			}
			collider->SetPos(position.x - collider->rect.w * 0.5F, position.y - collider->rect.h);
	}
	
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
	BROFILER_CATEGORY("Draw_EntityZombie.cpp", Profiler::Color::AliceBlue)
	SDL_Rect frameAnim = animation[(uint)state].GetCurrentFrame(dt);
	if (state== State_zomby::STATE_ATTACK)
	{
		int frameAttack = 8;
		if ((int)animation[(uint)state].current_frame == frameAttack)
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
	if (state == State_zomby::STATE_DEATH && animation[(uint)State_zomby::STATE_DEATH].Finished())
	{
		toDelete = true;
	}

	if (right)
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
	else
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
	int num = path.Count();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		showPath = !showPath;

	if (showPath)
	{
		for (uint i = 0; i < path.Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path.At(i)->x, path.At(i)->y);
			App->render->Blit(App->pathfinding->debug_tex, pos.x, pos.y);
		}

	}
		
}

bool EntityZombie::CleanUp()
{
	return true;
}



