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

j1Entity::j1Entity():j1Module()
{
	name.create("enemies");
}

bool j1Entity::Awake(pugi::xml_node&node)
{
	LOG("Init Entity");
	String_docXml.create(node.child_value());
	return true;
}

void j1Entity::Init()
{
	active = true;
}

bool j1Entity::Start()
{
	bool ret = true;
	//Loading file entity xml --------------------------------------------------------------
	pugi::xml_parse_result result = entity_file.load_file("enemies.xml");
	entity_node = entity_file.child("enemies");
	entity_texture = App->tex->Load("textures/enemies.png");

	if (entity_texture == nullptr)
	{
		LOG("Error loading player texture!");
		ret = false;
	}
	else
	{
		LOG("Loaded player texture succesfully");
	}

	if (result)
	{

		EntityIdle = LoadAnimations("idle");
		/*EntityWalk = LoadAnimations("walking");
		EntityDeath = LoadAnimations("death");
		EntityAttack = LoadAnimations("attack");*/

	}

	else
	{
		LOG("entity %s", result.description());
	}
	
	return ret;
}

Animation j1Entity::LoadAnimations(p2SString name)
{
	Animation anim_entity;
	SDL_Rect frameRect;

	for (pugi::xml_node enemies_frame = entity_file.child("enemies").child("enemy"); enemies_frame; enemies_frame = enemies_frame.next_sibling("enemy"))
	{
		pugi::xml_node e_node = entity_node.child("enemy").child("animation");
		for (pugi::xml_node frame = e_node.child(name.GetString()).child("frame"); frame; frame = frame.next_sibling("frame"))
		{
			frameRect.x = frame.attribute("x").as_int();
			frameRect.y = frame.attribute("y").as_int();
			frameRect.w = frame.attribute("width").as_int();
			frameRect.h = frame.attribute("height").as_int();
			anim_entity.PushBack(frameRect);
			LOG("CORRECT LOAD OF THE ANIMATION WITH: %i  , %i     ,%i       ,%i    COMPONENTS", frameRect.x, frameRect.y, frameRect.w, frameRect.h);
		}

	}
	anim_entity.speed = 0.1f;
	return anim_entity;
}

bool j1Entity::PreUpdate()
{
	return true;
}

bool j1Entity::Update(float dt)
{
	return true;
}

bool j1Entity::PostUpdate()
{
	

	return true;
}

bool j1Entity::Draw()
{
	bool ret = true;
	SDL_Rect CurrentFrame = EntityIdle.GetCurrentFrame();
	ret = App->render->Blit(entity_texture, App->player1->flPos.x - CurrentFrame.w / 2, App->player1->flPos.y - CurrentFrame.h, &CurrentFrame);
	return ret;
}

bool j1Entity::CleanUp()
{
	if (entity_texture != nullptr)
	{
		App->tex->UnLoad(entity_texture);
		entity_texture = nullptr;
	}

	return true;
}

bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&  nodePlayer) const
{
	return true;
}