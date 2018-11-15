#ifndef ENTITYBAT_H_
#define ENTITYBAT_H_
#include "j1Entity.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class EntityBat : public j1Entity
{
private:


	Animation anim_bat;


public:
	EntityBat(fPoint pos, Animation* anim, SDL_Texture* tex);
	~EntityBat();
	bool PreUpdate(float dt);
	void Move(float dt);
	void Draw() override;
	void OnCollision(Collider* collider);
	
};
#endif // !ENTITYBAT_H_


