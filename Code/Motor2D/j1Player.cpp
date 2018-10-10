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
	
	active = true;
}
bool j1Player::Start()
{
	bool ret = false;
	animState = AnimationState::ANIM_STATE_IDLE;
	//Loading file player xml --------------------------------------------------------------
	pugi::xml_document	player_file;
	pugi::xml_parse_result result = player_file.load_file(String_docXml.GetString());

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
		
		flPosPlayer.x = player_node.child("player1").attribute("Start_pos_x").as_float();
		flPosPlayer.y = player_node.child("player1").attribute("Start_pos_y").as_float();

		PlayerIdle = LoadAnimations("idle");
		PlayerWalk = LoadAnimations("walking");
		PlayerJump = LoadAnimations("jump");
		PlayerAttack = LoadAnimations("attack");
		PlayerDeath = LoadAnimations("death");
		PlayerSpawn = LoadAnimations("spawn");
		ret = CreateCol();
		currentTime = SDL_GetTicks();
		

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
		LOG("Animation: %s", name.GetString());
	}
	anim_aux.speed = 0.1f;
	return anim_aux;

}
inline bool j1Player::CreateCol()
{
	bool ret = false;
	offset.x = 3;
	offset.y = 0;
	SDL_Rect playerRect;
	playerRect.x = flPosPlayer.x+offset.x;
	playerRect.y = flPosPlayer.y;
	playerRect.w = player_node.child("player1").child("collider").attribute("w").as_int();
	playerRect.h = player_node.child("player1").child("collider").attribute("h").as_int();
	SDL_Rect pos = { flPosPlayer.x, flPosPlayer.y,1,1};
	ColliderPlayer = App->collision->AddCollider(playerRect, COLLIDER_PLAYER, App->player1);
	ColliderPos = App->collision->AddCollider(pos, COLLIDER_ENEMY, App->player1);
	if (ColliderPlayer != nullptr)
		ret = true;


	return ret;
}

bool j1Player::PreUpdate()
{
	//Player input-------------------------------------------------------------------
	if (animState != AnimationState::ANIM_STATE_SPAWN && animState != AnimationState::ANIM_STATE_DEATH)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE*/)
		{
			animState = AnimationState::ANIM_STATE_WALK;
			SpeedX = 0.5f;
			flPosPlayer.x += SpeedX;
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT /*&& App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE*/)
		{
			animState = AnimationState::ANIM_STATE_WALK;
			SpeedX = -0.5f;
			flPosPlayer.x += SpeedX;

		}
		
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_IDLE || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			animState = AnimationState::ANIM_STATE_IDLE;
			SpeedX = 0.0f;
		}

		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			animState = AnimationState::ANIM_STATE_DEATH;
			SpeedX = 0.0f;
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			flPosPlayer.y += 1;
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			flPosPlayer.y -= 1;
		}
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (!jumping)
			{
				jumping = true;
				currentTime = SDL_GetTicks();
			/*	flPosPlayer.y = flPosPlayer.y;*/
				initial_JumpVelosity = -50.0f;
			}
			
		}

	}
	


	
	//Gravity ------------------------------------------------------------------------
	
		/*JumpVelocity = (App->map->gravity*(SDL_GetTicks() - currentTime)) / 2;
		instantPos.y = initialPos.y + (initial_JumpVelosity *(SDL_GetTicks() - currentTime) + JumpVelocity);*/
	
	

	
	
	LOG("%f", initial_JumpVelosity);
	

	
	return true;
}
bool j1Player::Update(float dt)
{
	SDL_Rect CurrentFrame;

	if (animState == AnimationState::ANIM_STATE_IDLE)
	{
	   CurrentFrame = PlayerIdle.GetCurrentFrame();
	}
	if (animState == AnimationState::ANIM_STATE_WALK)
	{
		CurrentFrame = PlayerWalk.GetCurrentFrame();
	}
	if (animState == AnimationState::ANIM_STATE_DEATH)
	{
		CurrentFrame = PlayerDeath.GetCurrentFrame();
		if (PlayerDeath.Finished())
		{
			PlayerDeath.Reset();
			animState = AnimationState::ANIM_STATE_IDLE;
		}
		
		
			
		
	}
	if (animState == AnimationState::ANIM_STATE_SPAWN)
	{
		if (PlayerSpawn.Finished())
		{
			animState = AnimationState::ANIM_STATE_IDLE;
			PlayerSpawn.loop = 0;
		}

		else
		CurrentFrame = PlayerSpawn.GetCurrentFrame();

	}
	if(SpeedX<0.0f)
	App->render->Blit(ptexture, flPosPlayer.x-CurrentFrame.w/2, flPosPlayer.y-CurrentFrame.h,&CurrentFrame,SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(ptexture, flPosPlayer.x - CurrentFrame.w / 2, flPosPlayer.y - CurrentFrame.h, &CurrentFrame);

	PlayerMesure.x = CurrentFrame.w;
	PlayerMesure.y = CurrentFrame.h;

	ColliderPlayer->SetPos(flPosPlayer.x + offset.x - CurrentFrame.w / 2, flPosPlayer.y - CurrentFrame.h);
	ColliderPlayer->SetMeasurements(CurrentFrame.w-6, CurrentFrame.h);
	ColliderPos->SetPos(flPosPlayer.x , flPosPlayer.y );
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
void j1Player::SpawnPlayer()
{
	animState = AnimationState::ANIM_STATE_SPAWN;
	ColliderPlayer->type = COLLIDER_TYPE::COLLIDER_GOD;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{

	LOG("player wall");
	switch (c2->type)
	{
	case COLLIDER_WALL:

		//The player have collisioned with a side stand
		if (flPosPlayer.x <= c2->rect.x)
		{
			flPosPlayer.x = c2->rect.x- PlayerMesure.x/2;
		}
		else if(flPosPlayer.x >= c2->rect.x + c2->rect.w)
		{
			flPosPlayer.x = c2->rect.x + c2->rect.w + PlayerMesure.x / 2;
		}
		else
		{
			if (flPosPlayer.y > c2->rect.y + c2->rect.h)
			{
				flPosPlayer.y = c2->rect.y + c2->rect.h + PlayerMesure.y;

			}
			else
			{
				flPosPlayer.y = c2->rect.y ;
			}
		}
			
		
	
			break;
	default:
		break;
	}
	
	
	

}