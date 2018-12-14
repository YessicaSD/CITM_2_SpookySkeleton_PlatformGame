#include "j1StartMenu.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "UiItem_Label.h"
#include "j1Module.h"
#include "ModuleFadeToBack.h"
j1StartMenu::j1StartMenu()
{
	name.create("StartMenu");
}



bool j1StartMenu::Start()
{
	
	return true;
}

bool j1StartMenu::Update(float dt)
{
	

	App->render->Blit(Background, 0, 0, NULL, SDL_FLIP_NONE, 0.0F);
	
	return true;
}
bool j1StartMenu::CleanUp()
{
	
	return true;
}