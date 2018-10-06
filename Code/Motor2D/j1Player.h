#ifndef __ModulePlayer_H__
#define __ModulePlayer_H__

#include "j1Module.h"
#include "p2Animation.h"
#include "p2Point.h"
#include "j1Input.h"

class ModulePlayer : public j1Module
{
public:
	ModulePlayer();
	~ModulePlayer();

	bool Awake();
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();



};

#endif