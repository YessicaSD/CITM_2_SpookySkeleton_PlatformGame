#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "Brofiler\Brofiler.h"
#include "p2Point.h"


UiItem_Bar::UiItem_Bar(SDL_Rect hitBox, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot ) :UiItem(hitBox, parent, pivot)
{
	assert(parent != nullptr);
	frame_bar = *section;


}

void UiItem_Bar::Draw()
{
	BROFILER_CATEGORY("Draw_Bar.cpp", Profiler::Color::AliceBlue)
	App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pivot.x, hitBox.y - pivot.y, &frame_bar, SDL_FLIP_NONE, 0.0F);
}

