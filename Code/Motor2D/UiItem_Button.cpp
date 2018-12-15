#include "UiItem_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Point.h"
#include "j1Scene.h"
#include "ModuleFadeToBack.h"
#include "j1Map.h"

UiItem_Button::UiItem_Button(SDL_Rect hitBox, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click, const SDL_Rect * hover, p2Point<int> pivot ) : UiItem(hitBox, parent, pivot )
{
	assert(parent != nullptr);
	frames[IDLE] = *idle;

	if (click)
		frames[CLICK] = *click;
	else
		frames[CLICK] = *idle;

	if (hover)
		frames[HOVER] = *hover;
	else
		frames[HOVER] = *idle;
}

void UiItem_Button::AddFuntion(void(*funtionOnClick)())
{
	if (funtionOnClick != nullptr)
	{
		this->funtionOnClick = funtionOnClick;
	}
}
void UiItem_Button::AddFuntion(p2SString & string)
{
	if (&string != nullptr)
	{
		uint num = App->Gui->mapOfFuntions.Find(string);
		this->funtionOnClick = App->Gui->mapOfFuntions.At(num);
	}
}
void UiItem_Button::Draw()
{
	App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x-pivot.x, hitBox.y-pivot.y, &frames[state], SDL_FLIP_NONE, 0.0f);
}

void UiItem_Button::OnClickUp()
{
}

void UiItem_Button::OnClickDown()
{
	if (funtionOnClick)
	{
		funtionOnClick();
	}
}



