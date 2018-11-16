#ifndef ENTITYZOMBIE_H_
#define ENTITYZOMBIE_H_
#include "j1Entity.h"
#include "p2Point.h"

enum class State_zomby :uint
{
	STATE_IDLE,
	STATE_WALK,
	STATE_DEATH,
	STATE_ATTACK,
	STATE_MAX
};

class EntityZombie : public j1Entity
{
private:
	State_zomby state= State_zomby::STATE_IDLE;
	Animation animation[(uint)State_zomby::STATE_MAX];
	bool right	=true;
	iPoint playerPos;
	uint halfTileSize = 16;
	p2DynArray<iPoint> path;
public:
	EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex);
	void Draw() override;
	
	bool PreUpdate(float dt);
	void Move(float dt) override;
	bool CleanUp();
};
#endif // !ENTITYZOMBIE_H_
