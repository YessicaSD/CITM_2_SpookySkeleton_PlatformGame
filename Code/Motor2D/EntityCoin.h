#ifndef ENTITYCOIN_H_
#define ENTITYCOIN_H_
#include "j1Entity.h"
#include "j1Timer.h"
#include "p2Point.h"

struct SDL_Texture;
struct Collider;

enum class CoinState : uint
{
	STATE_IDLE,
	STATE_DEATH,
	STATE_MAX
};

class EntityCoin : public j1Entity
{

private:
	iPoint icoin_pos = { position.x,position.y };
	Animation anim_coin[(uint)CoinState::STATE_MAX];
	CoinState state = CoinState::STATE_IDLE;
	iPoint rectMesure = { 0,0 };
	bool batdeath_fx = false;
	j1Timer timer;
	int halfTileSize = 16;


public:

	EntityCoin(fPoint pos, Animation* anim, SDL_Texture* tex, entities_types type);
	~EntityCoin();
	bool PreUpdate(float dt) override;
	void Draw() override;
	void OnCollision(Collider* collider);
};



#endif