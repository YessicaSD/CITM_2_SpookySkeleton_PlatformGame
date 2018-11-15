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

}


bool EntityZombie::PreUpdate(float dt)
{
	return true;
}

void EntityZombie::Move()
{
}

bool EntityZombie::Draw(float dt)
{
	return true;
}

bool EntityZombie::CleanUp()
{
	return true;
}


//bool EntityZombie::Start()
//{
//	bool ret = true;
//	//Loading file entity xml --------------------------------------------------------------
//	pugi::xml_parse_result result = entity_file.load_file("enemies.xml");
//	entity_node = entity_file.child("enemies");
//	entity_texture = App->tex->Load("textures/enemies.png");
//
//	if (entity_texture == nullptr)
//	{
//		LOG("Error loading player texture!");
//		ret = false;
//	}
//	else
//	{
//		LOG("Loaded player texture succesfully");
//	}
//
//	if (result)
//	{
//		EntityIdle = LoadAnimations("idle");
//		EntityWalk = LoadAnimations("walk");
//		EntityDeath = LoadAnimations("death");
//		EntityAttack = LoadAnimations("attack");
//	}
//
//	else
//	{
//		LOG("entity %s", result.description());
//	}
//	return true;
//}
//
//
//Animation EntityZombie::LoadAnimations(p2SString name)
//{
//	Animation anim_entity;
//	SDL_Rect frameRect;
//
//	for (pugi::xml_node enemies_frame = entity_file.child("enemies").child("zombie"); enemies_frame; enemies_frame = enemies_frame.next_sibling("zombie"))
//	{
//		pugi::xml_node e_node = entity_node.child("zombie").child("animation");
//		for (pugi::xml_node frame = e_node.child(name.GetString()).child("frame"); frame; frame = frame.next_sibling("frame"))
//		{
//			frameRect.x = frame.attribute("x").as_int();
//			frameRect.y = frame.attribute("y").as_int();
//			frameRect.w = frame.attribute("width").as_int();
//			frameRect.h = frame.attribute("height").as_int();
//			anim_entity.PushBack(frameRect);
//			LOG("CORRECT LOAD OF THE ANIMATION BAT WITH: %i  , %i     ,%i       ,%i    COMPONENTS", frameRect.x, frameRect.y, frameRect.w, frameRect.h);
//		}
//
//	}
//	anim_entity.speed = 3.0f;
//	return anim_entity;
//}


