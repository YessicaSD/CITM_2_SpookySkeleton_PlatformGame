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
	bool ret = true;
	LOG("Init SDL player");
	
	pugi::xml_parse_result result = player_file.load_file(node.child_value());

	if( result == NULL)
	{
		LOG("player %s", result.description());
		return ret = false;
	}

	player_node = player_file.child("player");

	return ret;
}
void j1Player::Init()
{
	active = true;
}
bool j1Player::Start()
{
	bool ret = true;

	death_fx = true;
	jump_fx = true;
	
	if ((ptexture = App->tex->Load("textures/skeleton.png")) == nullptr) {
		LOG("Error loading player texture!");
		return ret = false;
	}
		

		Player_State[STATE_IDLE] = LoadAnimations("idle");
		Player_State[STATE_WALK] = LoadAnimations("walking");
		Player_State[STATE_JUMP] = LoadAnimations("jump");
		Player_State[STATE_ATTACK] = LoadAnimations("attack");
		Player_State[STATE_ATTACK].loop = false;
		Player_State[STATE_DEATH] = LoadAnimations("death");
		Player_State[STATE_DEATH].loop = false;
		Player_State[STATE_SPAWN] = LoadAnimations("spawn");
		Player_State[STATE_SPAWN].loop = false;
		ret = CreateCol();
		/*if (!loading)
		{
			flPos.x = App->map->returnPlayerPos().x;
			flPos.y = App->map->returnPlayerPos().y;
		}
		else
		{
			flPos.x = flplayerPosSaved.x;
			flPos.y = flplayerPosSaved.y;
		}*/
		/*distansToCam = App->map->returnCameraPos();*/

		App->render->camera.x = (flPos.x + distansToCam.x);
		App->render->camera.y = (flPos.y + distansToCam.y);

		maxSpeed = { player_node.child("player1").attribute("Speed_x").as_float(),
			player_node.child("player1").attribute("Speed_y").as_float() };

		speed = { 0.0F,  0.0F };
		loading = false;
	
	return ret;
}
 Animation j1Player::LoadAnimations(p2SString name)
{
	pugi::xml_node p1_node = player_node.child("player1").child("animation").child(name.GetString());
	Animation anim_aux;
	SDL_Rect frameRect;

	for (pugi::xml_node frame = p1_node.child("frame"); frame; frame = frame.next_sibling("frame"))
	{
		frameRect.x = frame.attribute("x").as_int();
		frameRect.y = frame.attribute("y").as_int();
		frameRect.w = frame.attribute("width").as_int();
		frameRect.h = frame.attribute("height").as_int();
		anim_aux.PushBack(frameRect);
		
	}
	anim_aux.speed = p1_node.attribute("anim_speed").as_float();

	return anim_aux;
}
 bool j1Player::CreateCol()
{
	bool ret = true;
	iPoint rectMesure = { player_node.child("player1").child("collider").attribute("w").as_int(),
		player_node.child("player1").child("collider").attribute("h").as_int() };

	SDL_Rect playerRect = { (flPos.x + rectMesure.x / 2), (flPos.y - rectMesure.y), rectMesure.x, rectMesure.y };

	
	if ((ColliderPlayer = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->player1)) == nullptr)
	{
		ret = false;
		LOG("FAIL CREATING PLAYER COL");
	}

	return ret;
}
bool j1Player::PreUpdate(float dt)
{
	moveDown = true;
	this->dt = dt;
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debugMode = !debugMode;

	if (!debugMode)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_JUMP || PlayerState == PlayerState::STATE_WALK)
			{
				right = true;
				speed.x = maxSpeed.x;
			}
			if (PlayerState == PlayerState::STATE_IDLE)
				PlayerState = PlayerState::STATE_WALK;
		}
		

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_JUMP || PlayerState == PlayerState::STATE_WALK)
			{
				right = false;
				speed.x = -maxSpeed.x;
			}
			if (PlayerState == PlayerState::STATE_IDLE)
				PlayerState = PlayerState::STATE_WALK;
			
		
		}
	

		if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN 
			&& (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_WALK)
			&& canJump)
				{
					PlayerState = PlayerState::STATE_JUMP;
					speed.y = -maxSpeed.y;
					canJump = false;
					App->audio->PlayFx(jump);
				
				}
			
		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN
			&& (PlayerState == PlayerState::STATE_IDLE || PlayerState == PlayerState::STATE_WALK))
			{
				speed.x = 0.0F;
				PlayerState = PlayerState::STATE_ATTACK;
			}

		if (PlayerState == PlayerState::STATE_DEATH)
		{
			if (speed != fPoint(0.0F, 0.0F))
				speed = fPoint(0.0F, 0.0F);

			if (death_fx)
			{
				App->audio->PlayFx(death_anim_fx);
				death_fx = false;
			}
		}
		
	}
	else
	{
		DebugModeInput();
	}
	
	ColliderPlayer->SetPos((flPos.x+speed.x * dt) - ColliderPlayer->rect.w / 2, (flPos.y + speed.y * dt) - ColliderPlayer->rect.h);
	return true;
}

void j1Player::OnCollision(Collider * c1, Collider * c2)
{
	Collider* colPlayer = c1;
	Collider* otherColl = c2;

	bool PlayerIsOn			= (int)flPos.y <= otherColl->rect.y && flPos.x + 4 > otherColl->rect.x && flPos.x - 4 < otherColl->rect.x + otherColl->rect.w;
	bool PlayerIsOnTheLeft	= flPos.x < otherColl->rect.x  && flPos.y > otherColl->rect.y;
	bool PlayerIsOnTheRight = flPos.x > otherColl->rect.x + otherColl->rect.w  && flPos.y > otherColl->rect.y;
	bool PlayerIsUnder		= flPos.y > otherColl->rect.y + otherColl->rect.h && colPlayer->rect.x + colPlayer->rect.w - 5 > otherColl->rect.x && colPlayer->rect.x + 5 < otherColl->rect.x + otherColl->rect.w;

		if (c2->type == COLLIDER_WALL)
		{
			if (PlayerIsOn)
			{
				if (PlayerState == PlayerState::STATE_JUMP)
					PlayerState = STATE_IDLE;

				App->player1->moveDown = false;
				App->player1->canJump = true;
				speed.y = (otherColl->rect.y - (int)flPos.y)/dt;
			}

			if (PlayerIsOnTheLeft)
			{
				speed.x = otherColl->rect.x - (flPos.x + colPlayer->rect.w/2)  ;
			}

			
			if (PlayerIsOnTheRight)
			{
				speed.x = (otherColl->rect.x + otherColl->rect.w) - (flPos.x - colPlayer->rect.w / 2);
			}

			
			if (PlayerIsUnder)
			{
				App->player1->speed.y = (otherColl->rect.y + otherColl->rect.h) - ((int)flPos.y- colPlayer->rect.h);
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
			{
				speed.x = 0.0F;
				if (PlayerState == PlayerState::STATE_WALK)
					PlayerState = PlayerState::STATE_IDLE;
			}
		}

		if (c2->type == COLLIDER_SPECIAL &&
				App->player1->speed.y >= 0 &&
					PlayerIsOn)
					{
						if (PlayerState == PlayerState::STATE_JUMP)
							PlayerState = STATE_IDLE;
						
						App->player1->moveDown = false;
						App->player1->canJump = true;
						speed.y = (otherColl->rect.y - (int)flPos.y) / dt;
						if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
						{
							speed.x = 0.0F;
							if (PlayerState == PlayerState::STATE_WALK)
								PlayerState = PlayerState::STATE_IDLE;
						}
							
		
					}
	
			


		if (otherColl->type == COLLIDER_ENEMY)
			App->player1->PlayerState = PlayerState::STATE_DEATH;
		

		if (otherColl->type == COLLIDER_RESPAWN)
			App->fade->FadeToBlack(App->map->num_thismaplvl);

		if (otherColl->type == COLLIDER_ICE &&
			App->player1->speed.y >= 0 &&
			PlayerIsOn)
		{
			if (PlayerState == PlayerState::STATE_JUMP)
				PlayerState = STATE_IDLE;

			App->player1->moveDown = false;
			App->player1->canJump = true;
			speed.y = (otherColl->rect.y - (int)flPos.y) / dt;
			speed.x +=  right ? 100*dt:  -100 * dt;
			

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
			{
					if (speed.x > 0.0F)
						speed.x -= 100 * dt;
					if (speed.x < 0.0F)
						speed.x += 100 * dt;

				

				if (PlayerState == PlayerState::STATE_WALK)
					PlayerState = PlayerState::STATE_IDLE;

			}
			
		}
	

}

bool j1Player::Update(float dt)
{
	if (!debugMode)
	{
		flPos.x += speed.x * dt;
		flPos.y += speed.y * dt;
		////Gravity ------------------------------------------------------------------------
		if (moveDown && !fading )
		{
			speed.y += App->map->level.gravity * dt ;
		
		}

	}

	if (!debugMode)
	{
		
		//Camera----------------------------------------------------------------------------------
		if ((flPos.x + distansToCam.x)* App->win->GetScale() > 0  && (App->map->level.tile_width*App->map->level.width) * App->win->GetScale() > (((flPos.x + distansToCam.x)* App->win->GetScale()) + App->render->camera.w) )
		{
			App->render->camera.x = ((flPos.x + distansToCam.x)* App->win->GetScale());

		}
		if(    (flPos.y + distansToCam.y) <= ((App->map->level.height * App->map->level.tile_height)*App->win->GetScale() - App->render->camera.h) && (flPos.y + distansToCam.y) > 0 )
		{
				App->render->camera.y = (flPos.y + distansToCam.y);
		}
			
	}

	ColliderPlayer->SetPos(flPos.x  - ColliderPlayer->rect.w / 2, flPos.y - ColliderPlayer->rect.h);
	return true;
}

bool j1Player::Draw(float dt)
{
	bool ret = true;
	SDL_Rect CurrentFrame;
	switch (PlayerState)
	{
	case PlayerState::STATE_IDLE:
		CurrentFrame = Player_State[STATE_IDLE].GetCurrentFrame(dt);
		break;
	case PlayerState::STATE_WALK:
		CurrentFrame = Player_State[STATE_WALK].GetCurrentFrame(dt);
		break;
	case PlayerState::STATE_ATTACK:
		CurrentFrame = Player_State[STATE_ATTACK].GetCurrentFrame(dt);
		if (Player_State[STATE_ATTACK].Finished())
		{
			PlayerState = PlayerState::STATE_IDLE;
			Player_State[STATE_ATTACK].Reset();
		}
		break;
	case PlayerState::STATE_DEATH:
		
		CurrentFrame = Player_State[STATE_DEATH].GetCurrentFrame(dt);
		if (Player_State[STATE_DEATH].Finished())
		{
			App->fade->FadeToBlack(App->map->num_thismaplvl);
		}
		
		break;
	case PlayerState::STATE_SPAWN:
		if (Player_State[STATE_SPAWN].Finished())
		{
			PlayerState = PlayerState::STATE_IDLE;
			Player_State[STATE_SPAWN].loop = 0;
		}
		else
			CurrentFrame = Player_State[STATE_SPAWN].GetCurrentFrame(dt);
		break;
	case PlayerState::STATE_JUMP:

		
		CurrentFrame = Player_State[STATE_JUMP].GetCurrentFrame(dt);
		break;
	default:
		ret = false;
		break;
	
	}
	
	if (right)
		ret = App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame);
	
	else	
		ret = App->render->Blit(ptexture, flPos.x - CurrentFrame.w / 2, flPos.y - CurrentFrame.h, &CurrentFrame, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	
	
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