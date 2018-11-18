#ifndef ENTITYBAT_H_
#define ENTITYBAT_H_
#include "j1Entity.h"
#include "j1Timer.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

enum class BatState : uint
{
	STATE_IDLE,
	STATE_DEATH,
	STATE_MAX
};

class EntityBat : public j1Entity
{
private:
	bool left = true;
	iPoint rectMesure = { 0,0 };
	iPoint ibat_pos = { position.x,position.y };
	fPoint speed_bat = { 0.0F,0.0F };
	Animation anim_bat[(uint)BatState::STATE_MAX];
	BatState state = BatState::STATE_IDLE;
	p2DynArray<iPoint> bat_path;
	int halfTileSize = 16;
	j1Timer timer;
	int i = 0;
	bool batdeath_fx = false;

public:
	EntityBat(fPoint pos, Animation* anim, SDL_Texture* tex, entities_types type);
	~EntityBat();
	bool PreUpdate(float dt) override;
	void Move(float dt);
	void Draw() override;
	void OnCollision(Collider* collider);
	
};
#endif // !ENTITYBAT_H_


