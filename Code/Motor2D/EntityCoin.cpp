#include "EntityCoin.h"
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

EntityCoin::EntityCoin(fPoint pos, Animation * anim, SDL_Texture * tex, entities_types type) :j1Entity(pos, tex, type)
{
	
	for (uint AnimNum = 0; AnimNum < (uint)CoinState::STATE_MAX; ++AnimNum)
	{
		anim_coin[AnimNum].speed = anim[AnimNum].speed;
		for (int j = 0; j < anim[AnimNum].numFrames; ++j)
		{
			anim_coin[AnimNum].PushBack(anim[AnimNum].ReturnFrame(j));
		}
	}
	pugi::xml_node nodeCoin = App->entity->entitiesNodeDoc.child("coin");
	pugi::xml_node nodeCol = nodeCoin.child("collider");
	rectMesure = { nodeCol.attribute("w").as_int(), nodeCol.attribute("h").as_int() };
	
	SDL_Rect playerCoin = { (pos.x - rectMesure.x / 2), (pos.y - rectMesure.y), rectMesure.x, rectMesure.y };
	collider = App->collision->AddCollider(playerCoin, COLLIDER_COIN, App->entity);
	coin_fx = true;
}

EntityCoin::~EntityCoin()
{
}

bool EntityCoin::PreUpdate(float dt)
{
	this->dt = dt;

	if (state == CoinState::STATE_DEATH && coin_fx)
	{
		App->audio->PlayFx(App->entity->fx_coin);
		coin_fx = false;
	}

	collider->SetPos((position.x + speed.x * dt) - collider->rect.w / 2, (position.y + speed.y * dt) - collider->rect.h);

	return true;
}



void EntityCoin::Draw()
{

	if (state == CoinState::STATE_DEATH && anim_coin[(uint)CoinState::STATE_DEATH].Finished())
	{
		toDelete = true;
	}

	SDL_Rect frameAnim = anim_coin[(uint)state].GetCurrentFrame(dt);
	App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim);
	
}

void EntityCoin::OnCollision(Collider * otherCollider)
{
	if (otherCollider->type == COLLIDER_PLAYER)
	{
		
			EntityCoin::state = CoinState::STATE_DEATH;
			collider->to_delete = true;
		
		
	}
}
