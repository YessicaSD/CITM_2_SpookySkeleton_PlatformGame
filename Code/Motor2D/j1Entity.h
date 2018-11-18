#ifndef __J1ENTITY_H_
#define __J1ENTITY_H_

#include "j1Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "p2Point.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

enum entities_types
{
	PLAYER = 0,
	ENEMY_BAT,
	ENEMI_ZOMBIE,
	UNKNOW,
};
class j1Entity 
{
protected: 
	SDL_Texture * texture = nullptr;
	float dt;
	bool showPath= false;
	
public:
	bool toDelete = false;
	fPoint position = {0.0F,0.0F};
	fPoint speed = { 0.0F,0.0F };
	Collider* collider = nullptr;
	entities_types type = UNKNOW;
public:
	j1Entity(fPoint position, SDL_Texture * tex, entities_types type);
	virtual ~j1Entity();
	

	virtual bool PreUpdate(float dt) { return true; };
	virtual void Move(float dt) {};
	virtual void Draw() {};
	virtual void OnCollision(Collider* collider) {};
	
};

#endif // __j1ENTITY_H__