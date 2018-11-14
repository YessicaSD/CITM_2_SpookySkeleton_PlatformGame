#include "EntityBat.h"

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
#include "j1Player.h"



EntityBat::EntityBat(fPoint pos) :j1Entity(pos)
{}
//bool EntityBat::Start()
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
//	}
//
//	else
//	{
//		LOG("entity %s", result.description());
//	}
//	return true;
//}

bool EntityBat::PreUpdate(float dt)
{
	return true;
}


Animation EntityBat::LoadAnimations(p2SString name)
{
	Animation anim_entity;
	SDL_Rect frameRect;

	for (pugi::xml_node enemies_frame = entity_file.child("enemies").child("bat"); enemies_frame; enemies_frame = enemies_frame.next_sibling("bat"))
	{
		pugi::xml_node e_node = entity_node.child("bat").child("animation");
		for (pugi::xml_node frame = e_node.child(name.GetString()).child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			frameRect.x = frame.attribute("x").as_int();
			frameRect.y = frame.attribute("y").as_int();
			frameRect.w = frame.attribute("width").as_int();
			frameRect.h = frame.attribute("height").as_int();
			anim_entity.PushBack(frameRect);
			LOG("CORRECT LOAD OF THE ANIMATION BAT WITH: %i  , %i     ,%i       ,%i    COMPONENTS", frameRect.x, frameRect.y, frameRect.w, frameRect.h);
		}

	}
	anim_entity.speed = 3.0F;
	return anim_entity;
}



void EntityBat::Move()
{

}

bool EntityBat::CleanUp()
{
	return true;
}

//bool EntityBat::CleanUp()
//{
//	if (entity_texture != nullptr)
//	{
//		App->tex->UnLoad(entity_texture);
//		entity_texture = nullptr;
//	}
//	return true;
//}



bool EntityBat::Draw(float dt)
{
//	bool ret = true;
//	SDL_Rect CurrentFrame = EntityIdle.GetCurrentFrame(dt);
//	ret = App->render->Blit(entity_texture, Bat_Pos.x, Bat_Pos.y, &CurrentFrame);
	return true;
}




