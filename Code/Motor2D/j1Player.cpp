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
		PlayerSpawn.loop = false;
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
		speed.x = 0.0F;
		speed.y = 0.0F;
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
	anim_aux.speed = 0.1F;

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
	this->moveDown = true;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		debugMode = !debugMode;
	}

	if (!debugMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (PlayerState == PlayerState::STATE_IDLE)
			{
				PlayerState = PlayerState::STATE_WALK;
				right = true;
				speed.x = 2.0F;
			}
			else if (PlayerState == PlayerState::STATE_JUMP)
			{
				right = true;
				speed.x = 2.0F;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
		{
			speed.x = 0.0F;
			if (PlayerState == PlayerState::STATE_WALK)
			{
				PlayerState = PlayerState::STATE_IDLE;
				
			}
	
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (PlayerState == PlayerState::STATE_IDLE)
			{
				PlayerState = PlayerState::STATE_WALK;
				right = false;
				speed.x = -2.0F;
			}
			else if (PlayerState == PlayerState::STATE_JUMP)
			{
				right = false;
				speed.x = -2.0F;
			}

		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			speed.x = 0;
			if (PlayerState == PlayerState::STATE_WALK)
			{
				PlayerState = PlayerState::STATE_IDLE;
			}
		}

		if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_WALK)
			{
				if (canJump)
				{
					PlayerState = PlayerState::STATE_JUMP;
					jump_fx = true;
					speed.y = -5.3F;
					canJump = false;
				}
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			if (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_WALK)
			{
				speed.x = 0;
				PlayerState = PlayerState::STATE_ATTACK;
			}
		}
	}
	else
	{
		DebugModeInput();
	}
	if (PlayerState == PlayerState::STATE_DEATH)
	{
		speed.x = 0.0F;
		speed.y = 0.0F;
	}

	ColliderPlayer->SetPos((flPos.x+speed.x) - 4, (flPos.y + speed.y) - 31);
	return true;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
		if (c2->type == COLLIDER_WALL)
		{
			Collider* wall = c2;
			Collider* colPlayer = c1;

			
			//The player is on the ground
			if (App->player1->flPos.y - colPlayer->rect.h / 3 <= wall->rect.y && colPlayer->rect.x <= wall->rect.x + wall->rect.w   && colPlayer->rect.x + colPlayer->rect.w >= wall->rect.x)
			{
				if (PlayerState == PlayerState::STATE_JUMP)
				{
					PlayerState = STATE_IDLE;
				}

				App->player1->moveDown = false;
				speed.y = 0.0F;
				App->player1->canJump = true;

			}


			// The player collide with the left side of the wall
			if (App->player1->flPos.x < wall->rect.x  && App->player1->flPos.y > wall->rect.y)
			{
				App->player1->SetPosPlayer_x(wall->rect.x - colPlayer->rect.w / 2);
			}

			// The player collide with the left side of the wall
			if (App->player1->flPos.x > wall->rect.x + wall->rect.w  && App->player1->flPos.y > wall->rect.y)
			{
				App->player1->SetPosPlayer_x(wall->rect.x + wall->rect.w + colPlayer->rect.w / 2);
			}

			// The player is under the wall
			if (App->player1->flPos.y > wall->rect.y + wall->rect.h && colPlayer->rect.x + colPlayer->rect.w - 5 > wall->rect.x && colPlayer->rect.x + 5< wall->rect.x + wall->rect.w)
			{
				App->player1->SetPosPlayer_y(wall->rect.y + wall->rect.h + colPlayer->rect.h);
				App->player1->speed.y = 0.0f;
			}
		}

		//if (c1->type == COLLIDER_SPECIAL)
		//{
		//	for (p2List_item<Object_Layer*>* item_special = data.collition_layers.start; item_special; item_special = item_special->next)
		//	{
		//		if (App->player1->speed.y > item_special->data->special_coll)
		//		{
		//			Collider* wall = c1;
		//			Collider* colPlayer = c2;
		//			//The player is on the wall
		//			if (App->player1->flPos.y - colPlayer->rect.h / 3 <= wall->rect.y && colPlayer->rect.x <= wall->rect.x + wall->rect.w   && colPlayer->rect.x + colPlayer->rect.w >= wall->rect.x)
		//			{
		//				App->player1->moveDown = false;
		//				App->player1->speed.y = 0.0f;
		//				App->player1->SetPosPlayer_y(wall->rect.y + 1);
		//				App->player1->canJump = true;
		//			}
		//		}
		//	}
		//}


	/*	if (c1->type == COLLIDER_ENEMY)
		{
			App->player1->PlayerState = PlayerState::STATE_DEATH;
		}

		if (c1->type == COLLIDER_RESPAWN)
		{
			App->fade->FadeToBlack(App->map->num_thismaplvl);
		}*/
	

}

bool j1Player::Update(float dt)
{
	if (!debugMode)
	{
		flPos += speed;
	
		////Gravity ------------------------------------------------------------------------
		if (moveDown && !fading && speed.y < 3.0F)
		{
			speed.y += App->map->data.gravity;
		
		}

	}

	if (!debugMode)
	{
		
		//Camera----------------------------------------------------------------------------------
		if ((flPos.x + distansToCam.x)* App->win->GetScale() > 0  && (App->map->data.tile_width*App->map->data.width) * App->win->GetScale() > (((flPos.x + distansToCam.x)* App->win->GetScale()) + App->render->camera.w) )
		{
			App->render->camera.x = ((flPos.x + distansToCam.x)* App->win->GetScale());

		}
		if(    (flPos.y + distansToCam.y) <= ((App->map->data.height * App->map->data.tile_height)*App->win->GetScale() - App->render->camera.h) && (flPos.y + distansToCam.y) > 0 )
		{
				App->render->camera.y = (flPos.y + distansToCam.y);
		}
			
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
	bool ret = true;
	SDL_Rect CurrentFrame;
	switch (PlayerState)
	{
	case PlayerState::STATE_IDLE:
		CurrentFrame = PlayerIdle.GetCurrentFrame();
		break;
	case PlayerState::STATE_WALK:
		CurrentFrame = PlayerWalk.GetCurrentFrame();
		break;
	case PlayerState::STATE_ATTACK:
		CurrentFrame = PlayerAttack.GetCurrentFrame();
		if (PlayerAttack.Finished())
		{
		
			PlayerState = PlayerState::STATE_IDLE;
		}
		break;
	case PlayerState::STATE_DEATH:
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
	case PlayerState::STATE_SPAWN:
		if (PlayerSpawn.Finished())
		{
			PlayerState = PlayerState::STATE_IDLE;
			PlayerSpawn.loop = 0;
		}

		else
			CurrentFrame = PlayerSpawn.GetCurrentFrame();
		break;
	case PlayerState::STATE_JUMP:

		if (jump_fx)
		{
			App->audio->PlayFx(jump);
			jump_fx = false;
		}
		CurrentFrame = PlayerJump.GetCurrentFrame();
		break;
	default:
		ret = false;
		break;
	
	}
	
	if (speed.x<0.0F)
		ret=App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	
	else
		ret= App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame);

	
	
	return ret;
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





void j1Player::DebugModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		PlayerState = PlayerState::STATE_WALK;
		speed.y = -5.0f;
		flPos.y += speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		PlayerState = PlayerState::STATE_WALK;
		speed.y = +5.0f;
		flPos.y += speed.y;
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		PlayerState = PlayerState::STATE_WALK;
		speed.x = -5.0f;
		flPos.x += speed.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		PlayerState = PlayerState::STATE_WALK;
		speed.x = 5.0f;
		flPos.x += speed.x;
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
	play.append_attribute("x");
	play.attribute("x").set_value(flPos.x);

	play.append_attribute("y");
	play.attribute("y").set_value(flPos.y);
	return true;
}