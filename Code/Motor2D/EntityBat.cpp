#include "EntityBat.h"
#include "j1Entity.h"

#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"


#include "j1Input.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "ModuleFadeToBack.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Player.h"

EntityBat::EntityBat(fPoint pos,Animation* anim, SDL_Texture* tex):j1Entity(pos,tex)
{






}

EntityBat::~EntityBat()
{
}


bool EntityBat::PreUpdate(float dt)
{
	return true;
}








void EntityBat::Move(float dt)
{

}




void EntityBat::Draw()
{

	
}

void EntityBat::OnCollision(Collider * collider)
{
}




