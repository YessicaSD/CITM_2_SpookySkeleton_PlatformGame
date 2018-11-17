#ifndef ENTITYZOMBIE_H_
#define ENTITYZOMBIE_H_
#include "j1Entity.h"
#include "p2Point.h"
#include "j1Timer.h"
#include "j1Collision.h"
enum class State_zomby :uint
{
	STATE_IDLE,
	STATE_WALK,
	STATE_DEATH,
	STATE_ATTACK,
	STATE_MAX
};
enum direciton
{
	RIGHT, 
	LEFT, 
	FALL,
	NONE
};
class EntityZombie : public j1Entity
{
private:
	State_zomby state= State_zomby::STATE_IDLE;
	Animation animation[(uint)State_zomby::STATE_MAX];
	bool right		=	true;
	bool moveDown	=	true;
	iPoint playerPos;
	uint halfTileSize = 16;
	p2DynArray<iPoint> path;
	j1Timer timer;
	j1Entity* entityPlayerTarget = nullptr;
	uint pathIndex = 0;
	uint speedModule = 10;
	Collider* colAttack = nullptr;

public:
	EntityZombie(fPoint pos, Animation* anim, SDL_Texture* tex, entities_types type);
	~EntityZombie()
	{
		if(colAttack!=nullptr)
		colAttack->to_delete = true;
	}
	void Draw() override;
	
	bool PreUpdate(float dt) override;
	void Move(float dt) override;
	void OnCollision(Collider * otherColl) override;
	bool CleanUp();

};
#endif // !ENTITYZOMBIE_H_
