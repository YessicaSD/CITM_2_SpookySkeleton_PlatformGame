#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
j1Player::j1Player() : j1Module()
{
	active = false;
	name.create("player");
}
bool j1Player:: Awake (pugi::xml_node &node) 
{
	LOG("Init SDL player");
	
	String_docXml.create( node.child_value());
	
	return true;
}

bool j1Player::Start()
{
	bool ret = false;
	//Loading file player xml --------------------------------------------------------------
	pugi::xml_document	player_file;
	pugi::xml_parse_result result = player_file.load_file(String_docXml.GetString());

	if (result)
	{
		player_node = player_file.child("player");
		LoadAnimations();
		ret = true;

	}

	else
	{
		LOG("player %s", result.description());
		return false;
	}

	return true;
}
bool j1Player::LoadAnimations()
{
	bool ret = true;
	pugi::xml_node p1_node = player_node.child("player1").child("animation");

	
	ptexture = App->tex->Load("textures/skeleton.png");


	if (ptexture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	else {
		LOG("Loaded player texture succesfully");
	}

	for (pugi::xml_node frame = p1_node.child("idle").child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		SDL_Rect frameRect;
		frameRect.x = frame.attribute("x").as_int();
		frameRect.y = frame.attribute("y").as_int();
		frameRect.w = frame.attribute("width").as_int();
		frameRect.h = frame.attribute("height").as_int();
		PlayerIdle.PushBack(frameRect);
	}

	return ret;

}