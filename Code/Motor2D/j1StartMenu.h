#ifndef _START_MENU_H_
#define _START_MENU_H_
#include "j1Module.h"
#include "j1Textures.h"
class j1StartMenu:public j1Module
{
private:
	SDL_Texture * Background = nullptr;
public:
	j1StartMenu();
	bool Start() override;
	bool Update(float dt) override;
};


#endif // !_START_MENU_H_

