#include "Player.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1ModuleEntity.h"
#include "j1Input.h"
#include "j1Window.h"
#include "ModuleFadeToBack.h"
#include "Scene.h"
#include "Brofiler\Brofiler.h"

void Player::DebugModeInput()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		speed.y = -5.0F;
		position.y += speed.y*dt*30;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		speed.y = +5.0F;
		position.y += speed.y*dt*30;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		speed.x = -5.0F;
		position.x += speed.x*dt*30;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		speed.x = 5.0F;
		position.x += speed.x*dt*30;
	}
}

Player::Player(fPoint position, Animation* anim, SDL_Texture* tex, entities_types type):j1Entity(position,tex, type)
{
	BROFILER_CATEGORY("PlayerContructor", Profiler::Color::Red)
	for (uint i = 0; i < STATE_MAX; ++i)
	{
		animation[i].speed = anim[i].speed;
		for (int j = 0; j<anim[i].numFrames;++j)
		{
			animation[i].PushBack(anim[i].ReturnFrame(j));
		}
	}

	animation[STATE_SPAWN].loop = false;
	animation[STATE_DEATH].loop = false;
	animation[STATE_ATTACK].loop = false;

	pugi::xml_node nodePlayer = App->entity->entitiesNodeDoc.child("player1");
	pugi::xml_node nodeCol= nodePlayer.child("collider");

	iPoint rectMesure = { 0,0 };
	rectMesure = {nodeCol.attribute("w").as_int(), nodeCol.attribute("h").as_int() };

	SDL_Rect playerRect = { (position.x - rectMesure.x / 2), (position.y - rectMesure.y), rectMesure.x, rectMesure.y };
	collider = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->entity);

	if (App->scene->num_thismaplvl == 1)
	{
		distansToCam = { (int)App->map->level.properties.Get("Distant_to_cam_x"),(int)App->map->level.properties.Get("Distant_to_cam_y") };
		App->render->camera.x = (position.x + distansToCam.x);
		App->render->camera.y = (position.y + distansToCam.y);
		LOG("DISTANCE TO CAM EN Y ES %i", distansToCam.y);
	}

	if (App->scene->num_thismaplvl == 2)
	{
		distansToCam = { -60,-180 };
		App->render->camera.x = (position.x + distansToCam.x);
		App->render->camera.y = (position.y + distansToCam.y);
		LOG("DISTANCE TO CAM EN Y ES %i", distansToCam.y);
	}

	
	
	death_fx = true;
	jump_fx = true;
	maxSpeed = { nodePlayer.attribute("Speed_x").as_float(), nodePlayer.attribute("Speed_y").as_float() };

}

Player::~Player()
{
}

bool Player::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Player.cpp", Profiler::Color::Salmon)
	this->dt = dt;
	if (collider == nullptr || collider->to_delete==true)
	{
		state = STATE_DEATH;
	}
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		debugMode = !debugMode;
		collider->type = (collider->type==COLLIDER_GOD)? COLLIDER_PLAYER:COLLIDER_GOD;
	}
		

	if (!debugMode)
	{
		


		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN
			&& (state == STATE_IDLE || state == STATE_WALK)
			&& canJump)
		{
			state = STATE_JUMP;
			speed.y = -maxSpeed.y;
			canJump = false;
			if (jump_fx)
			{
				App->audio->PlayFx(App->entity->fx_jump);
				jump_fx = false;
			}
		

		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (state == STATE_IDLE || state == STATE_JUMP || state == STATE_WALK)
			{
				if (!right)
					right = true;
				if (speed.x < maxSpeed.x)
				{
					speed.x += acceleration_x * dt;
				}

			}
			if (state == STATE_IDLE)
				state = STATE_WALK;
		}
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (state == STATE_IDLE || state == STATE_JUMP || state == STATE_WALK)
			{
				if (right)
					right = false;
				if (speed.x > -maxSpeed.x)
				{
					speed.x -= acceleration_x * dt;
				}

			}
			if (state == STATE_IDLE)
				state = STATE_WALK;
		}

		if (state == STATE_JUMP)
			jump_fx = true;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN
			&& (state == PlayerState::STATE_IDLE || state == PlayerState::STATE_WALK))
		{
			speed.x = 0.0F;
			state = STATE_ATTACK;
		}

		if (state == STATE_DEATH)
		{
			if (speed != fPoint(0.0F, 0.0F))
				speed = fPoint(0.0F, 0.0F);


			App->fade->FadeToBlack(App->scene->num_thismaplvl);
			if (death_fx)
			{
				App->audio->PlayFx(App->entity->fx_death);
				death_fx = false;
			}
		}

		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
		{
			
				if ((int)speed.x != 0)
					speed.x += (speed.x>0) ? -100 * dt : 100 * dt;
				else
					speed.x = 0.0F;

				if (state == STATE_WALK)
					state = STATE_IDLE;

			
			
		}
	}
	else
	{
		DebugModeInput();
	}

	collider->SetPos((position.x + speed.x * dt) - collider->rect.w*0.5F, (position.y + speed.y * dt) - collider->rect.h);
	return true;
}

void Player::Move(float dt)
{
	BROFILER_CATEGORY("Move_Player.cpp", Profiler::Color::Black)
	if (!debugMode)
	{
		
		position.x += speed.x * dt;
		position.y += speed.y * dt;
		////Gravity ------------------------------------------------------------------------
			speed.y += App->map->level.gravity * dt;

		//Camera----------------------------------------------------------------------------------
		if ((position.x + distansToCam.x)* App->win->GetScale() > 0 && (App->map->level.tile_width*App->map->level.width) * App->win->GetScale() > (((position.x + distansToCam.x)* App->win->GetScale()) + App->render->camera.w))
			App->render->camera.x = ((position.x + distansToCam.x)* App->win->GetScale());

		if ((position.y + distansToCam.y) <= ((App->map->level.height * App->map->level.tile_height)*App->win->GetScale() - App->render->camera.h) && (position.y + distansToCam.y) > 0)
			App->render->camera.y = (position.y + distansToCam.y);

	}

	collider->SetPos(position.x - collider->rect.w / 2, position.y - collider->rect.h);
}

void Player::Draw()
{
	BROFILER_CATEGORY("Draw_Player.cpp", Profiler::Color::AliceBlue)
	SDL_Rect frameAnim = animation[state].GetCurrentFrame(dt);
	if (animation[STATE_SPAWN].Finished())
	{
		state = STATE_IDLE;
		animation[STATE_SPAWN].Reset();
	}
	if (animation[STATE_ATTACK].Finished())
	{
		state = STATE_IDLE;
		animation[STATE_ATTACK].Reset();
	}
	

	if(right)
		App->render->Blit(texture, position.x - frameAnim.w/2, position.y - frameAnim.h, &frameAnim);
	else
		App->render->Blit(texture, position.x - frameAnim.w / 2, position.y - frameAnim.h, &frameAnim, SDL_FLIP_HORIZONTAL);
}

void Player::OnCollision(Collider * otherColl)
{
	BROFILER_CATEGORY("PlayerOnCollision", Profiler::Color::Red)
	bool PlayerIsOn = (int)position.y <= otherColl->rect.y 
		&& (int)position.x >= otherColl->rect.x
		&& (int)position.x <= otherColl->rect.x + otherColl->rect.w;
	bool PlayerIsOnTheLeft = position.x <= otherColl->rect.x  && (int)position.y > otherColl->rect.y;
	bool PlayerIsOnTheRight = position.x >= otherColl->rect.x + otherColl->rect.w  && (int)position.y > otherColl->rect.y;
	bool PlayerIsUnder = position.y > otherColl->rect.y + otherColl->rect.h && collider->rect.x + collider->rect.w - 5 > otherColl->rect.x && collider->rect.x + 5 < otherColl->rect.x + otherColl->rect.w;
	
	if (otherColl->type == COLLIDER_WALL)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
		{
			speed.x = 0;
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			speed.x = maxSpeed.x;
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			speed.x = -maxSpeed.x;
		}
				
	}

	if (otherColl->type == COLLIDER_WALL|| otherColl->type == COLLIDER_ICE || otherColl->type == COLLIDER_SPECIAL)
	{
		if (otherColl->type != COLLIDER_ICE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE)
		{
			speed.x = 0.0F;
			if (state == STATE_WALK)
				state = STATE_IDLE;
		}

		
	
		if ((PlayerIsOn && otherColl->type != COLLIDER_SPECIAL) || (speed.y >= 0 && otherColl->type == COLLIDER_SPECIAL && PlayerIsOn))
		{
			
			if (state == STATE_JUMP)
				state = STATE_IDLE;

			
			canJump = true;
			speed.y = (otherColl->rect.y - (int)position.y) / dt;

			
		
		}
		if (otherColl->type != COLLIDER_SPECIAL)
		{
			if (PlayerIsOnTheLeft)
				speed.x = otherColl->rect.x - ((int)position.x + collider->rect.w / 2);


			if (PlayerIsOnTheRight)
				speed.x = (otherColl->rect.x + otherColl->rect.w) - (position.x - collider->rect.w / 2);
			
			if (PlayerIsUnder)
				speed.y = (otherColl->rect.y + otherColl->rect.h) - ((int)position.y - collider->rect.h);
		}
		
	}

	if (otherColl->type==COLLIDER_ENTITY && state == STATE_ATTACK)
	{
		otherColl->to_delete = true;
	}

	if (otherColl->type == COLLIDER_ENEMY && state != STATE_ATTACK)
		state = STATE_DEATH;


	if (otherColl->type == COLLIDER_RESPAWN)
		App->fade->FadeToBlack(App->scene->num_thismaplvl);

	

}

