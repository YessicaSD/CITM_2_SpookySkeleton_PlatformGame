#include "j1StartMenu.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Gui.h"
#include "UiItem_Label.h"
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
	SDL_Rect ButtonFrames[3];
	ButtonFrames[0] = { 374,0,253,161 };
	ButtonFrames[1] = { 374,161,253,161 };
	ButtonFrames[2] = { 374,322,253,161 };
	App->Gui->AddButton({ 388,402,252,146 },(const SDL_Rect*) &ButtonFrames[0], (const SDL_Rect*)&ButtonFrames[2], (const SDL_Rect*)&ButtonFrames[1]);
	App->Gui->AddButton({ 392,565,252,146 }, (const SDL_Rect*)&ButtonFrames[0], (const SDL_Rect*)&ButtonFrames[2], (const SDL_Rect*)&ButtonFrames[1]);
	ButtonFrames[0] = { 186,0,70,70 };
	ButtonFrames[1] = { 256,0,70,70 };
	ButtonFrames[2] = { 627,0,70,70 };
	App->Gui->AddButton({915,31,70,70}, (const SDL_Rect*)&ButtonFrames[0], (const SDL_Rect*)&ButtonFrames[1], (const SDL_Rect*)&ButtonFrames[2]);
	ButtonFrames[0] = { 0,0,93,93 };
	ButtonFrames[1] = { 93,0,93,93 };
	App->Gui->AddButton({ 51,35,65,65 }, (const SDL_Rect*)&ButtonFrames[0], (const SDL_Rect*)&ButtonFrames[1], (const SDL_Rect*)&ButtonFrames[1], {14,12});
	UiItem_Label* label = App->Gui->AddLabel("Play", { 62,32,28,255 }, App->Gui->arrayFonts[COPPERPLATE_B_I_48], {430,460});
	SDL_Color color = {113,57,36,255};
	label->ChangeTextureHoverOrIDLE(NULL, &color, NULL);

	label = App->Gui->AddLabel("Continue", { 62,32,28,255 }, App->Gui->arrayFonts[COPPERPLATE_B_I_24], { 440,635 });
	label->ChangeTextureHoverOrIDLE(NULL, &color, NULL);


	return true;
}

bool j1StartMenu::Update(float dt)
{
	App->render->Blit(Background, 0, 0, NULL, SDL_FLIP_NONE, 0.0F);
	
	return true;
}
