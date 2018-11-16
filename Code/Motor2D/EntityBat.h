#ifndef ENTITYBAT_H_
#define ENTITYBAT_H_
#include "j1Entity.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

enum class BatState : uint
{
	STATE_IDLE,
	STATE_MAX
};

class EntityBat : public j1Entity
{
private:
	bool right = true;
	iPoint rectMesure = { 0,0 };
	iPoint ibat_pos = { position.x,position.y };
	Animation anim_bat;
	BatState state = BatState::STATE_IDLE;
	const p2DynArray<iPoint>* bat_path;
	
public:
	EntityBat(fPoint pos, Animation* anim, SDL_Texture* tex);
	~EntityBat();
	bool PreUpdate(float dt) override;
	void Move(float dt);
	void Draw() override;
	void OnCollision(Collider* collider);
	
};
#endif // !ENTITYBAT_H_


