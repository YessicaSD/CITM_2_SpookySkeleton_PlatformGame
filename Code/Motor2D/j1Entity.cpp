#include "j1Entity.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "p2Point.h"
#include "j1App.h"
#include "j1Audio.h"

#include "j1Collision.h"
#include "j1Render.h"
#include "Brofiler\Brofiler.h"


j1Entity::j1Entity(fPoint position, SDL_Texture* tex, entities_types type):position(position),texture(tex),type(type)
{}

j1Entity::~j1Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}


bool j1Entity::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Entity.cpp", Profiler::Color::Salmon)
	return true;
}


void j1Entity::Draw()
{
	/*if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame(App->dt)));*/
	
}



void j1Entity::OnCollision(Collider * collider)
{
}


