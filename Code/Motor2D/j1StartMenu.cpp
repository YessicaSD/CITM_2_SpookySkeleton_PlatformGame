#include "j1StartMenu.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Gui.h"
j1StartMenu::j1StartMenu()
{
	name.create("StartMenu");
}

bool j1StartMenu::Start()
{
	Background = App->tex->Load("textures/StartMenu/Background.png");
	App->win->scale = 1.0F;
	SDL_Rect Rect = { 0,93,374,377 };
	App->Gui->AddImage({ 328,28,374,377 }, &Rect);
	
	return true;
}

bool j1StartMenu::Update(float dt)
{
	App->render->Blit(Background, 0, 0, NULL, SDL_FLIP_NONE, 0.0F);
	
	return true;
}
