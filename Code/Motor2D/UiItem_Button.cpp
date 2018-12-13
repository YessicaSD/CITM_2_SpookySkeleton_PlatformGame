#include "UiItem_Button.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "p2Point.h"

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

void UiItem_Button::Draw()
{
	App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x-pivot.x, hitBox.y-pivot.y, &frames[state], SDL_FLIP_NONE, 0.0f);
}

void UiItem_Button::OnClickUp()
{
}

void UiItem_Button::OnClickDown()
{
}

