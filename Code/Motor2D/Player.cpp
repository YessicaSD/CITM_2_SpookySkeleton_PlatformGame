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
#include "j1Scene.h"
#include "UiItem_Label.h"
#include "j1Pathfinding.h"
#include "Brofiler/Brofiler.h"
#include <cmath>

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

void Player::HandleGroundCollider(Collider* col)
{
	LOG("HANDLE GROUND COLLITION");
}

void Player::HandleBodyCollider(Collider* col)
{
}

Player::Player(fPoint position, Animation* anim, SDL_Texture* tex, entities_types type):j1Entity(position,tex, type)
{
	maxHeight = 70.F; 
	jumpDuration = 0.35F;
	jumpInitialVelocity = -((2 * maxHeight) / jumpDuration);
	gravity = -(-(2 * maxHeight) / (jumpDuration * jumpDuration));

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

	SDL_Rect groundRect = { (position.x - rectMesure.x / 2), (position.y - groundHeight * 0.5 ), rectMesure.x, groundHeight};
	groundCollider = App->collision->AddCollider(groundRect, COLLIDER_GROUND_PLAYER, App->entity);

	if (App->scene->num_thismaplvl == 1)
	{
		distansToCam = { (int)App->map->level.properties.GetAsFloat("Distant_to_cam_x"),(int)App->map->level.properties.GetAsFloat("Distant_to_cam_y") };
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

	lastPos = position;
	get_hurt = true;
	maxSpeed = { nodePlayer.attribute("Speed_x").as_float(), nodePlayer.attribute("Speed_y").as_float() };
	iceSpeed = maxSpeed.x + maxSpeed.x * 0.75;
	maxSpeed.x = 120;
	App->scene->coin_points = 0;
	str_coin.create("x %u", App->scene->coin_points);
	App->scene->label_coin->ChangeTextureIdle(&str_coin, NULL, NULL);

	App->scene->points = 0;
	str_points.create("POINTS : %u", App->scene->points);
	App->scene->label_points->ChangeTextureIdle(&str_points, NULL, NULL);
}
bool Player::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Player.cpp", Profiler::Color::Salmon)
	this->dt = dt;

	static bool death_fx = true;

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		debugMode = !debugMode;
		collider->type = (collider->type==COLLIDER_GOD)? COLLIDER_PLAYER:COLLIDER_GOD;
	}
	
	if (!debugMode)
	{
		if (collider == nullptr || collider->to_delete == true)
		{
			state = STATE_DEATH;
		}
		if (state != STATE_SPAWN && state != STATE_DEATH)
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN )
			{
				if (isGrounded)
				{
					state = STATE_JUMP;
					speed.y = jumpInitialVelocity;

					App->audio->PlayFx(App->entity->fx_jump);
				}
				
			}

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN
				&& (state == PlayerState::STATE_IDLE || state == PlayerState::STATE_WALK))
			{
				speed.x = 0.0F;
				state = STATE_ATTACK;
			}
			
			float horizontalInput = App->input->GetHorizontal();
			if (horizontalInput < 0)
			{
				right = false;
			}
			else if(horizontalInput > 0)
			{
				right = true;
			}
			if (isGrounded)
			{
				
				if (horizontalInput != 0.0f)
				{
					if (state == STATE_IDLE)
						state = STATE_WALK;
				}
				else
				{
					state = STATE_IDLE;
				}

				if (onIce)
				{
					speed.x += 10 * horizontalInput;
					if (std::abs(speed.x) > iceSpeed)
					{
						speed.x = iceSpeed * horizontalInput;
					}
				}
				else
				{
					speed.x = horizontalInput * maxSpeed.x;
				}

			}
			else
			{
				speed.x += horizontalInput * 3.5;
			}
			
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

			if (get_hurt)
			{
				if (App->scene->player_lives > 0)
				{
					App->scene->player_lives--;
					App->scene->coin_points = 0;
				}
				else
				{
					LOG("PLAYER LIVES ARE %u", App->scene->player_lives);
					App->scene->points = 0;
					App->scene->coin_points = 0;
					App->scene->player_lives = 3;
					App->map->active = false;
					App->pathfinding->active = false;
					App->scene->state = SceneState::STARTMENU;
					
				}
					

				get_hurt = false;
			}
			
		}

	}
	else
	{
		DebugModeInput();
	}
	fPoint temporalPosition((position.x + speed.x * dt), (position.y + (speed.y * dt) + (0.5F * gravity * dt * dt)));
	collider->SetPos(temporalPosition.x - collider->rect.w*0.5F, temporalPosition.y - collider->rect.h);
	groundCollider ->SetPos(temporalPosition.x - collider->rect.w * 0.5F, temporalPosition.y - groundHeight * 0.5);
	isGrounded = false;
	onIce = false;
	return true;
}

void Player::Move(float dt)
{
	BROFILER_CATEGORY("Move_Player.cpp", Profiler::Color::Black)
	lastPos = position;
	if (!debugMode)
	{
		LOG("Speed x: %f  --- Speed y: %f", speed.x, speed.y);
		position.x += speed.x * dt;
		position.y += (speed.y * dt) + (0.5F * gravity * dt * dt);
		////Gravity ------------------------------------------------------------------------
		speed.y += (gravity * dt);
		
	}

	//Camera----------------------------------------------------------------------------------
	if ((position.x + distansToCam.x) * App->win->GetScale() > 0 && (App->map->level.tile_width * App->map->level.width) * App->win->GetScale() > (((position.x + distansToCam.x) * App->win->GetScale()) + App->render->camera.w))
		App->render->camera.x = ((position.x + distansToCam.x) * App->win->GetScale());

	if ((position.y + distansToCam.y) <= ((App->map->level.height * App->map->level.tile_height) * App->win->GetScale() - App->render->camera.h) && (position.y + distansToCam.y) > 0)
		App->render->camera.y = (position.y + distansToCam.y);

	collider->SetPos(position.x - collider->rect.w * 0.5f, position.y - collider->rect.h);
	groundCollider->SetPos(position.x - collider->rect.w * 0.5F, position.y - groundHeight * 0.5);
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
	//SDL_Rect pos = { position.x,position.y,1,1 };
	//App->render->DrawQuad(pos, 255, 0, 0, 255);
}

void Player::OnCollision(Collider * otherColl)
{
	BROFILER_CATEGORY("PlayerOnCollision", Profiler::Color::Red)
	bool PlayerIsOn = lastPos.y - 1  <= otherColl->rect.y || position.y <= otherColl->rect.y
		&& (int)position.x+collider->rect.w*0.5F >= otherColl->rect.x
		&& (int)position.x-collider->rect.w*0.5F <= otherColl->rect.x + otherColl->rect.w;

	bool PlayerIsOnTheLeft = false;
	bool PlayerIsOnTheRight = false;
	bool PlayerIsUnder = false;

	if (PlayerIsOn == false)
	{
		PlayerIsOnTheLeft = position.x <= otherColl->rect.x  && position.y > otherColl->rect.y;
		PlayerIsOnTheRight = position.x >= otherColl->rect.x + otherColl->rect.w  && position.y > otherColl->rect.y;
		PlayerIsUnder = position.y > otherColl->rect.y + otherColl->rect.h && collider->rect.x + collider->rect.w - 5 > otherColl->rect.x && collider->rect.x + 5 < otherColl->rect.x + otherColl->rect.w;
	}
	
	if (otherColl->type == COLLIDER_WALL || otherColl->type == COLLIDER_ICE)
	{
		if (PlayerIsOnTheLeft)
		{
			if (dt != 0.0F)
				speed.x = (int)(otherColl->rect.x - (position.x + collider->rect.w * 0.5)) / dt;
			else
				speed.x = 0.0F;
		}
		if (PlayerIsOnTheRight)
		{
			if (dt != 0.0F)
				speed.x = (int)(otherColl->rect.x + otherColl->rect.w - (position.x - collider->rect.w * 0.5)) / dt;
			else
				speed.x = 0.0F;
		}
		
	}
	

	if (PlayerIsUnder)
	{
		if (otherColl->type == COLLIDER_WALL || otherColl->type == COLLIDER_ICE)
			speed.y = (otherColl->rect.y + otherColl->rect.h) - ((int)position.y - collider->rect.h);

	}


	if ((otherColl->type == COLLIDER_ENEMY && state != STATE_ATTACK) || otherColl->type == COLLIDER_RESPAWN)
		state = STATE_DEATH;

	if (otherColl->type == COLLIDER_ENTITY && state == STATE_ATTACK)
	{
		App->scene->points += 200;
		str_points.create("POINTS : %u", App->scene->points);
		App->scene->label_points->ChangeTextureIdle(&str_points, NULL, NULL);
		otherColl->to_delete = true;
	}

	if (otherColl->type == COLLIDER_COIN)
	{
		App->scene->coin_points++;
		App->scene->points += 50;

		str_coin.create("x %u", App->scene->coin_points);
		App->scene->label_coin->ChangeTextureIdle(&str_coin, NULL, NULL);

		str_points.create("POINTS : %u", App->scene->points);
		App->scene->label_points->ChangeTextureIdle(&str_points, NULL, NULL);
	}


}

void Player::OnCollisionGround(Collider* otherColl)
{
	if (groundCollider->rect.y <= otherColl->rect.y || lastPos.y <= otherColl->rect.y)
	{
		isGrounded = true;
		speed.y = 0;
		position.y = otherColl->rect.y;
		if (state == STATE_JUMP)
			state = STATE_IDLE;
	}
	
	if (otherColl->type == COLLIDER_ICE)
		onIce = true;
}

