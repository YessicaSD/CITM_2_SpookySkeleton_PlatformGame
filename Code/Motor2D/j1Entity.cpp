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

j1Entity::j1Entity():j1Module()
{
	name.create("entity");
}

bool j1Entity::Awake(pugi::xml_node&node)
{
	LOG("Init Entity");
	String_docXml.create(node.child_value());
	return true;
}

void j1Entity::Init()
{

}

bool j1Entity::Start()
{
	bool ret = false;
	//Loading file entity xml --------------------------------------------------------------
	pugi::xml_parse_result result = entity_file.load_file(String_docXml.GetString());
	entity_node = entity_file.child("enemies");
	entity_texture = App->tex->Load(entity_node.child("image").attribute("source").value());

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
		entity_node = entity_file.child("enemies");

	}

	else
	{

	}
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
		}

	}
	anim_entity.speed = 0.1f;
	return anim_entity;
}

bool j1Entity::PreUpdate()
{

}

bool j1Entity::Update(float dt)
{

}

bool j1Entity::PostUpdate()
{

}

bool j1Entity::Draw()
{

}

bool j1Entity::CleanUp()
{

}

bool j1Entity::Load(pugi::xml_node&)
{

}

bool j1Entity::Save(pugi::xml_node&  nodePlayer) const
{

}