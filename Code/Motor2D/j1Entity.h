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

class j1Entity 
{
protected: 
	Animation * animation = nullptr;
	Collider* collider = nullptr;

public:
	fPoint position = {0.0F,0.0F};
	iPoint speed = { 0.0F,0.0F };
	SDL_Texture * texture = nullptr;

public:
	j1Entity(fPoint position);
	virtual ~j1Entity();
	

	virtual bool PreUpdate(float dt);
	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	
};

#endif // __j1ENTITY_H__