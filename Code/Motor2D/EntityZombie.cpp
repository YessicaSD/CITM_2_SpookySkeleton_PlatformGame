#include "EntityZombie.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
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
}


bool EntityZombie::PreUpdate(float dt)
{
	this->dt = dt;
	return true;
}

void EntityZombie::Move()
{

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
}

bool EntityZombie::CleanUp()
{
	return true;
}



