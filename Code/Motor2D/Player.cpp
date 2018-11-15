#include "Player.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Map.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1ModuleEntity.h"
Player::Player(fPoint position, Animation* anim, SDL_Texture* tex):j1Entity(position,anim,tex)
{
	SDL_Rect col = { position.x,position.y,8,32 };
	collider = App->collision->AddCollider(col, COLLIDER_PLAYER, App->entity);
}

Player::~Player()
{
}

bool Player::PreUpdate(float dt)
{
	this->dt = dt;
	return true;
}

void Player::Draw()
{
	SDL_Rect rect = { 2,0,14,31 };
	App->render->Blit(texture, position.x, position.y, &rect);
	SDL_Rect frameAnim = animation[state].GetCurrentFrame(dt);
	App->render->Blit(texture, position.x, position.y, &frameAnim);
}

void Player::OnCollision(Collider * collider)
{
}

