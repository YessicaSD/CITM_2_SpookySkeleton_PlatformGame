#include "j1Player.h"
#include "p2Log.h"

#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Collision.h"
#include "j1Map.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "ModuleFadeToBack.h"
j1Player::j1Player() : j1Module()
{
	active = false;
	name.create("player");
}
bool j1Player:: Awake (pugi::xml_node &node) 
{
	LOG("Init SDL player");
	death_anim_fx = App->audio->LoadFx("audio/fx/smw_stomp_bones.wav");
	jump = App->audio->LoadFx("audio/fx/jump.wav");
	death = App->audio->LoadFx("audio/fx/death.wav");
	String_docXml.create( node.child_value());
	
	return true;
}
void j1Player::Init()
{
	
	active = true;
}
bool j1Player::Start()
{
	bool ret = false;
	//Loading file player xml --------------------------------------------------------------
	pugi::xml_document	player_file;
	pugi::xml_parse_result result = player_file.load_file(String_docXml.GetString());
	death_fx = true;
	jump_fx = true;
	ptexture = App->tex->Load("textures/skeleton.png");
	

	if (ptexture == nullptr) {
		LOG("Error loading player texture!");
		ret = false;
	}
	else {
		LOG("Loaded player texture succesfully");
	}

	if (result)
	{
		player_node = player_file.child("player");
		
		PlayerIdle = LoadAnimations("idle");
		PlayerWalk = LoadAnimations("walking");
		PlayerJump = LoadAnimations("jump");
		PlayerAttack = LoadAnimations("attack");
		PlayerAttack.loop = false;
		PlayerDeath = LoadAnimations("death");
		PlayerDeath.loop = false;
		PlayerSpawn = LoadAnimations("spawn");
		ret = CreateCol();
		if (!loading)
		{
			flPos.x = App->map->returnPlayerPos().x;
			flPos.y = App->map->returnPlayerPos().y;
		}
		else
		{

			flPos.x = flplayerPosSaved.x;
			flPos.y = flplayerPosSaved.y;
		}
		distansToCam.x = App->map->returnCameraPos().x;
		distansToCam.y = App->map->returnCameraPos().y;
		App->render->camera.x = ((flPos.x + distansToCam.x));
		App->render->camera.y = ((flPos.y + distansToCam.y));
		Speed.x = 0.0f;
		Speed.y = 0.0f;
		animState = AnimationState::ANIM_STATE_SPAWN;
		fading = false;
		loading = false;
	}

	else
	{
		LOG("player %s", result.description());
		return ret;
	}

	return ret;
}
 Animation j1Player::LoadAnimations(p2SString name)
{
	bool ret = true;
	pugi::xml_node p1_node = player_node.child("player1").child("animation");
	Animation anim_aux;

	SDL_Rect frameRect;
	for (pugi::xml_node frame = p1_node.child(name.GetString()).child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		frameRect.x = frame.attribute("x").as_int();
		frameRect.y = frame.attribute("y").as_int();
		frameRect.w = frame.attribute("width").as_int();
		frameRect.h = frame.attribute("height").as_int();
		anim_aux.PushBack(frameRect);
		
	}
	anim_aux.speed = 0.1f;
	return anim_aux;

}
 bool j1Player::CreateCol()
{
	bool ret = false;
	offset.x = 3;
	offset.y = 0;
	SDL_Rect playerRect;
	playerRect.x = flPos.x+offset.x;
	playerRect.y = flPos.y;
	playerRect.w = player_node.child("player1").child("collider").attribute("w").as_int();
	playerRect.h = player_node.child("player1").child("collider").attribute("h").as_int();

	ColliderPlayer = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->player1);
	
	if (ColliderPlayer != nullptr)
		ret = true;


	return ret;
}
bool j1Player::PreUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (debugMode)
			debugMode = false;
		else
			debugMode = true;
	}
	this->moveDown = true;
	return true;
}
bool j1Player::Update(float dt)
{
	
	if (!debugMode)
	{
		//Player input-------------------------------------------------------------------
		if (animState != AnimationState::ANIM_STATE_SPAWN && animState != AnimationState::ANIM_STATE_DEATH)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE*/)
			{
				animState = AnimationState::ANIM_STATE_WALK;
				Speed.x = 1.0f;
				flPos.x += Speed.x;
				
			}

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE*/)
			{
				animState = AnimationState::ANIM_STATE_WALK;
				Speed.x = -1.0f;
				flPos.x += Speed.x;

			}


			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			{
				animState = AnimationState::ANIM_STATE_IDLE;
				Speed.x = 0.0f;
			}
		

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE*/)
			{
				if (moveDown)
				{
					animState = AnimationState::ANIM_STATE_WALK;

					flPos.y += 0.5;
				}


			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				jump_fx = true;
				if (!jumping)
				{
					Speed.y = -5.2f;
					jumping = true;
				}



			}

			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
			{
				animState = AnimationState::ANIM_STATE_ATTACK;
			}

		}

		if (jumping)
		{

			if (Speed.y != 0.0f)
			{
				Speed.y += 0.1f;
			}

		}
		flPos.y += Speed.y;

		//Gravity ------------------------------------------------------------------------
		if (moveDown && !fading)
		{

			flPos.y += App->map->data.gravity;
		}
	
	

		if ((flPos.x + distansToCam.x)* App->win->GetScale() > 0  && (App->map->data.tile_width*App->map->data.width) * App->win->GetScale() > (((flPos.x + distansToCam.x)* App->win->GetScale()) + App->render->camera.w) )
		{
			App->render->camera.x = ((flPos.x + distansToCam.x)* App->win->GetScale());

		}
		if(    (flPos.y + distansToCam.y) <= ((App->map->data.height * App->map->data.tile_height)*App->win->GetScale() - App->render->camera.h) && (flPos.y + distansToCam.y) > 0 )
		{
				App->render->camera.y = (flPos.y + distansToCam.y);
		}
			
			
		
		
	}
	else
	{
	 DebugModeInput();
	}
	
	ColliderPlayer->SetPos(flPos.x - 4, flPos.y - 31);

	return true;
}

bool j1Player::PostUpdate()
{
	return true;
};
bool j1Player::Draw()
{
	SDL_Rect CurrentFrame;
	switch (animState)
	{
	case AnimationState::ANIM_STATE_IDLE:
		CurrentFrame = PlayerIdle.GetCurrentFrame();
		break;
	case AnimationState::ANIM_STATE_WALK:
		CurrentFrame = PlayerWalk.GetCurrentFrame();
		break;
	case AnimationState::ANIM_STATE_ATTACK:
		attack = true;
		break;
	case AnimationState::ANIM_STATE_DEATH:
		if (death_fx)
		{
			App->audio->PlayFx(death_anim_fx);
			death_fx = false;
		}
		CurrentFrame = PlayerDeath.GetCurrentFrame();
		if (PlayerDeath.Finished())
		{
			App->fade->FadeToBlack(App->map->num_thismaplvl);
		}
		
		break;
	case AnimationState::ANIM_STATE_SPAWN:
		if (PlayerSpawn.Finished())
		{
			animState = AnimationState::ANIM_STATE_IDLE;
			PlayerSpawn.loop = 0;
		}

		else
			CurrentFrame = PlayerSpawn.GetCurrentFrame();
		break;
	}
	if (attack)
	{
		CurrentFrame = PlayerAttack.GetCurrentFrame();
		if (PlayerAttack.Finished())
		{
			attack = false;
		}
	}
	if (!attack)
	{
		PlayerAttack.Reset();
	}
	if (jumping)
	{
		
		if (jump_fx)
		{
			App->audio->PlayFx(jump);
			jump_fx = false;
		}
		CurrentFrame = PlayerJump.GetCurrentFrame();
	}
	if (!jumping)
	{
		PlayerJump.Reset();
	}
	if (Speed.x<0.0f)
		App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	
	else
		App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame);

	
	
	return true;
}
bool j1Player::CleanUp()
{
	App->tex->UnLoad(ptexture);
	
	
	if (ColliderPlayer!=nullptr)
	{
		ColliderPlayer->to_delete = true;
		ColliderPlayer = nullptr;
	}

	
	return true;
}
void j1Player::SpawnPlayer()
{
	animState = AnimationState::ANIM_STATE_SPAWN;
	ColliderPlayer->type = COLLIDER_TYPE::COLLIDER_GOD;
}




void j1Player::DebugModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		Speed.y = -5.0f;
		flPos.y += Speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		Speed.y = +5.0f;
		flPos.y += Speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		Speed.x = -5.0f;
		flPos.x += Speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		animState = AnimationState::ANIM_STATE_WALK;
		Speed.x = 5.0f;
		flPos.x += Speed.x;
	}
}
bool j1Player::Load(pugi::xml_node& nodePlayer)
{
	App->player1->loading = true;
	flplayerPosSaved.x = nodePlayer.child("position").attribute("x").as_float();
	flplayerPosSaved.y = nodePlayer.child("position").attribute("y").as_float();
	App->fade->FadeToBlack(App->map->SavedLevel);
	return true;
}
bool j1Player::Save(pugi::xml_node&  nodePlayer) const
{
	pugi::xml_node play = nodePlayer.append_child("position");
	play.attribute("x").set_value(flPos.x);
	play.attribute("y").set_value(flPos.y);
	return true;
}