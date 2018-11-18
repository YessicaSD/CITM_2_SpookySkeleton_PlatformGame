#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:
	uint num_thismaplvl = 1;
	bool loadingSaveFile = false;
private:
	pugi::xml_document	sceneFile;
	pugi::xml_node sceneNode;
	pugi::xml_node saveNode;
public:
	bool loadedLeve = true;
	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&) override;

	// Called before the first frame
	bool Start()override;

	// Called before all Updates
	bool PreUpdate(float dt)override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp()override;

	bool Load(pugi::xml_node&) override;
	

	bool Save(pugi::xml_node&) const override;
	

private:
	SDL_Texture * debug_tex;
};

#endif // __j1SCENE_H__
