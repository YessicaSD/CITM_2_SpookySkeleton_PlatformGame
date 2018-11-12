#ifndef __J1ENTITY_H_
#define __J1ENTITY_H_

#include "j1Module.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "p2List.h"
#include "SDL/include/SDL_render.h"

struct SDL_Texture;
struct Collider;

class j1Entity 
{
protected: 
	Animation * animation = nullptr;
	Collider* collider = nullptr;

public:
	iPoint position;

public:
	j1Entity(int x, int y);
	virtual ~j1Entity();
	const Collider* GetCollider() const;

	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	
};

#endif // __j1ENTITY_H__