#ifndef __j1COLLITION_H__
#define __j1COLLITION_H__

#include "p2SString.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

#define MAX_COLLIDERS 320

enum COLLIDER_TYPE
{
	COLLIDER_IGNORE_HIT = 0,//A collider that doesn't generate any collision
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_GOD,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	int damage = 1;
	COLLIDER_TYPE type;
	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetDamage(int dmg)
	{
		damage = dmg;
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	void SetMeasurements(int w, int h)
	{
		rect.w = w;
		rect.h = h;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};


class j1Collision : public j1Module 
{
private:
	int getFilledColNum();
	Collider* colliders[MAX_COLLIDERS] = { nullptr };
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;

public:

	j1Collision();
	~j1Collision();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback);
	bool CheckCollision(const SDL_Rect& r) const;
	

	void Init()
	{
		name.create("collition");
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	 bool Start()
	{
		return true;
	}

	// Called each loop iteration
	 bool PreUpdate();
	

	// Called each loop iteration
	bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();



	bool Load(pugi::xml_node&)
	{
		return true;
	}

	bool Save(pugi::xml_node&) const
	{
		return true;
	}


};

#endif // __j1MODULE_H__
