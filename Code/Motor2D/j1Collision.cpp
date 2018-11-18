#include "j1Collision.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "SDL/include/SDL_keyboard.h"
#include "Brofiler/Brofiler.h"


j1Collision::j1Collision()
{
	name.create("collision");
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	for (uint i =0; i<COLLIDER_MAX; ++i)
	{
		for (uint j = 0; j<COLLIDER_MAX; ++j)
		{
			matrix[i][j] = false;
		}
	}

	//Collider wall-----------------------------------------------------
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENTITY] = true;

	//Collider special-------------------------------------------------
	matrix[COLLIDER_SPECIAL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_SPECIAL][COLLIDER_GOD] = true;
	matrix[COLLIDER_SPECIAL][COLLIDER_ENTITY] = true;

	//Collider player---------------------------------------------------
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_RESPAWN] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SPECIAL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ICE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENTITY] = true;
	
	//Collider enemy -----------------------------------------------------
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;

	//Collider God--------------------------------------------------------
	matrix[COLLIDER_GOD][COLLIDER_WALL] = true;
	matrix[COLLIDER_GOD][COLLIDER_SPECIAL] = true;
	matrix[COLLIDER_GOD][COLLIDER_ICE] = true;
	//Collider respawn ---------------------------------------------------
	matrix[COLLIDER_RESPAWN][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_RESPAWN][COLLIDER_ENTITY] = true;

	//Collider ice -------------------------------------------------------
	matrix[COLLIDER_ICE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ICE][COLLIDER_ENTITY] = true;
	//Collider entity ----------------------------------------------------
	matrix[COLLIDER_ENTITY][COLLIDER_ICE] = true;
	matrix[COLLIDER_ENTITY][COLLIDER_WALL] = true;
	matrix[COLLIDER_ENTITY][COLLIDER_RESPAWN] = true;
	matrix[COLLIDER_ENTITY][COLLIDER_SPECIAL] = true;
	matrix[COLLIDER_ENTITY][COLLIDER_PLAYER] = true;

}
j1Collision::~j1Collision()
{

}
bool j1Collision:: PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Collisiom.cpp", Profiler::Color::Salmon)
	// Calculate collisions
	Collider* c1 = nullptr;
	Collider* c2 = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{

				if (matrix[c1->type][c2->type] && c1->callback)
				{
					c1->callback->OnCollision(c1, c2);
				}

				if (matrix[c2->type][c1->type] && c2->callback)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}

		}
	}
	
	return true;
}
bool j1Collision::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_Collision.cpp", Profiler::Color::MediumSlateBlue)
	// Remove all colliders scheduled for deletion---------------------------------------------
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return true;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_IGNORE_HIT: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_WALL: // blue
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER: // green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENEMY: // red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;

		case COLLIDER_RESPAWN: // red
			App->render->DrawQuad(colliders[i]->rect, 125,125, 0, alpha);
			break;
		case COLLIDER_SPECIAL: //purple
			App->render->DrawQuad(colliders[i]->rect, 125, 0, 125, alpha);
			break;
		case COLLIDER_ICE:
			App->render->DrawQuad(colliders[i]->rect, 255, 50, 0, alpha);
			break;
		case COLLIDER_GOD:
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
			break;
		case COLLIDER_ENTITY:
			App->render->DrawQuad(colliders[i]->rect, 25, 25, 255, alpha);
			break;


		}
	}
	return true;
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	uint i;
	for (i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			return ret;//Does the same as a break, but allows us to skip the last if
		}
	}

	//If we get to max colliders, pop an error
	if (i == MAX_COLLIDERS)
	{
		LOG("-------------------------------------Max colliders reached!\n");
	}

	return ret;
}
int j1Collision::getFilledColNum()
{
	int filledColNum = 0;

	for (int i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			filledColNum++;
		}
	}

	return filledColNum;
}

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	//Return true if there is an overlap between argument "r" and property "rect"
	return (this->rect.x + this->rect.w > r.x   &&
		this->rect.x                < r.x + r.w &&
		this->rect.y + this->rect.h > r.y       &&
		this->rect.y < r.y + r.h);
}