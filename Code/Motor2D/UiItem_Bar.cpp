#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "UiItem_Image.h"
#include "Brofiler\Brofiler.h"
#include "p2Point.h"


UiItem_Bar::UiItem_Bar(SDL_Rect hitBox, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot ) :UiItem(hitBox, parent, pivot)
{
	assert(parent != nullptr);
	frame_bar = *section;


	SDL_Rect Rect_thumb_volume = { 646,140,51,52 };
	thumb = App->Gui->AddImage({ 157,-15,51,52 }, &Rect_thumb_volume, this, { 0,0 });
	thumb->draggable = true;


}

void UiItem_Bar::Draw(const float& dt)
{
	BROFILER_CATEGORY("Draw_Bar.cpp", Profiler::Color::AliceBlue)
	App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pivot.x, hitBox.y - pivot.y, &frame_bar, SDL_FLIP_NONE, 0.0F);
}

float UiItem_Bar::GetBarValue()
{
	float ipos_bar = thumb->hitBox.x + (thumb->hitBox.w/2);
	float fixed_pos = hitBox.x + (thumb->hitBox.w / 2);
	float fpos_bar = hitBox.x + hitBox.w - (thumb->hitBox.w / 2);
	float final_pos = (ipos_bar - fixed_pos) / (fpos_bar-fixed_pos);
	LOG("FINAL POSITION OF THE THUMB IS %.2f", final_pos);
	return final_pos;
}

