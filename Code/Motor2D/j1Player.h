
#ifndef __MODULE_PLAYER_H__
#define __MODULE_PLAYER_H__

#include "PugiXml\src\pugixml.hpp"
#include "j1Module.h"

#include "p2SString.h"
#include "p2Point.h"
#include "Animation.h"


class j1App;
struct SDL_Texture;

class j1Player : public j1Module
{
private:
	
	fPoint initialPos;
	fPoint instantPos;
	fPoint offset;

	float SpeedX=0.0f;
	Uint32 currentTime;
	Animation PlayerIdle;

	pugi::xml_node player_node;
	SDL_Texture* ptexture=nullptr;
	p2SString String_docXml;
	Collider* ColliderPlayer=nullptr;

public:
	bool activeGravity=true;


public:
	j1Player();
	void Init();
	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();
	 bool LoadAnimations();
	 bool CreateCol();
	bool PostUpdate();

	/*bool Load(pugi::xml_node&);*/
	//bool Save(pugi::xml_node&) const;

};

#endif // __j1MODULE_H__
