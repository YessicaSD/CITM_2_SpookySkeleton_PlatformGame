#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Map.h"
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
void j1Player::Init()
{
	instantPos.x = 0;
	instantPos.y = 0;
	active = true;
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
		ret = LoadAnimations();
		ret = CreateCol();
		
		

	}

	else
	{
		LOG("player %s", result.description());
		return ret;
	}

	return ret;
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

	SDL_Rect frameRect;
	for (pugi::xml_node frame = p1_node.child("idle").child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		
		frameRect.x = frame.attribute("x").as_int();
		frameRect.y = frame.attribute("y").as_int();
		frameRect.w = frame.attribute("width").as_int();
		frameRect.h = frame.attribute("height").as_int();
		PlayerIdle.PushBack(frameRect);
	}
	PlayerIdle.speed = 0.005f;
	return ret;

}
inline bool j1Player::CreateCol()
{
	bool ret = false;
	offset.x = 3;
	offset.y = 0;
	SDL_Rect playerRect;
	playerRect.x = instantPos.x+offset.x;
	playerRect.y = instantPos.y;
	playerRect.w = player_node.child("player1").child("collider").attribute("w").as_int();
	playerRect.h = player_node.child("player1").child("collider").attribute("h").as_int();

	ColliderPlayer = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->player1);
	if (ColliderPlayer != nullptr)
		ret = true;
	return ret;
}

bool j1Player::PreUpdate()
{
	//Player input-------------------------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
	{
		SpeedX += 0.5f;
		currentTime = SDL_GetTicks();
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP )
	{
		if (SpeedX > 0)
		{
			SpeedX -= 0.5f;
			initialPos.x = instantPos.x;
		}
		
	}
		

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
	{
		SpeedX -= 0.5f;
		currentTime = SDL_GetTicks();
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE)
	{
		if (SpeedX < 0)
		{
			SpeedX += 0.5f;
			initialPos.x = instantPos.x;
		}
		
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		instantPos.y -= 0.5f;
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		instantPos.y += 0.25f;

	//Gravity ------------------------------------------------------------------------
	if(activeGravity)
	instantPos.y = instantPos.y + App->map->gravity;


	instantPos.x = initialPos.x + SpeedX* ( SDL_GetTicks()-currentTime);

	ColliderPlayer->SetPos(instantPos.x+offset.x, instantPos.y);
	return true;
}
bool j1Player::Update(float dt)
{

	App->render->Blit(ptexture,instantPos.x,instantPos.y,&PlayerIdle.GetCurrentFrame(),1.0f);
	
	return true;
}

bool j1Player::PostUpdate()
{
	activeGravity = true;
	return true;
};
bool j1Player::CleanUp()
{
	App->tex->UnLoad(ptexture);
	ColliderPlayer->to_delete = true;
	return true;
}