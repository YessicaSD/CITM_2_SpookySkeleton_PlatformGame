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
	SDL_Texture * texture = nullptr;
	float dt;
public:
	fPoint position = {0.0F,0.0F};
	fPoint speed = { 0.0F,0.0F };
	Collider* collider = nullptr;
public:
	j1Entity(fPoint position, SDL_Texture * tex);
	virtual ~j1Entity();
	

	virtual bool PreUpdate(float dt);
	virtual void Move(float dt) {};
	virtual void Draw();
	virtual void OnCollision(Collider* collider);
	
};

#endif // __j1ENTITY_H__