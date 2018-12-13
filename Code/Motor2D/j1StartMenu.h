#ifndef _START_MENU_H_
#define _START_MENU_H_
#include "j1Module.h"
#include "p2List.h"
#include "UiItem.h"
struct  SDL_Texture;

class UiItem_Image;

class j1StartMenu:public j1Module
{
private:
	SDL_Texture * Background = nullptr;
	UiItem_Image* titleImage = nullptr;
	uint fx_death_aux;
	p2List<UiItem*> thisMenuItems;
public:
	j1StartMenu();
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;
};


#endif // !_START_MENU_H_

