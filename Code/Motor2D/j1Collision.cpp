#include "j1Collision.h"

#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "SDL\include\SDL_keyboard.h"


j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;
	//Collider none-----------------------------------------------------
	matrix[COLLIDER_IGNORE_HIT][COLLIDER_IGNORE_HIT] = false;
	matrix[COLLIDER_IGNORE_HIT][COLLIDER_WALL] = false;
	matrix[COLLIDER_IGNORE_HIT][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_IGNORE_HIT][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_IGNORE_HIT][COLLIDER_GOD] = false;

	//Collider wall-----------------------------------------------------
	matrix[COLLIDER_WALL][COLLIDER_IGNORE_HIT] = false;
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_WALL][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_WALL][COLLIDER_GOD] = true;

	//Collider player---------------------------------------------------
	matrix[COLLIDER_PLAYER][COLLIDER_IGNORE_HIT] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_GOD] = false;


	//Collider enemy ----------------------------------------------
	matrix[COLLIDER_ENEMY][COLLIDER_IGNORE_HIT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_WALL] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_GOD] = false;

	//Collider God--------------------------------------------------------
	matrix[COLLIDER_GOD][COLLIDER_IGNORE_HIT] = false;
	matrix[COLLIDER_GOD][COLLIDER_WALL] = false;
	matrix[COLLIDER_GOD][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GOD][COLLIDER_ENEMY] = true;


}
j1Collision::~j1Collision()
{

}
bool j1Collision:: PreUpdate()
{
	//LOG("Filled colliders %i", getFilledColNum());
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
	// Remove all colliders scheduled for deletion---------------------------------------------
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}
bool j1Collision::PostUpdate()
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
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