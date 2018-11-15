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
	bool right=true;

public:
	EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex);
	void Draw() override;
	
	bool PreUpdate(float dt);
	void Move();
	bool CleanUp();
};
#endif // !ENTITYZOMBIE_H_
