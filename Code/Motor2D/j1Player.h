
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
	
	Animation PlayerIdle;
	pugi::xml_node player_node;
	SDL_Texture* ptexture=nullptr;
	p2SString String_docXml;

public:

	j1Player();
	

	void Init()
	{
		active = true;
	}

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool LoadAnimations();
	//// Called each loop iteration
	//bool PreUpdate();

	//// Called each loop iteration
	//bool Update(float dt);

	//// Called each loop iteration
	//bool PostUpdate();

	//// Called before quitting
	//bool CleanUp();

	/*bool Load(pugi::xml_node&);*/

	//bool Save(pugi::xml_node&) const;

};

#endif // __j1MODULE_H__
