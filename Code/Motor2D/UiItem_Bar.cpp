#include "UiItem_Bar.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UiItem.h"
#include "UiItem_Image.h"
#include "Brofiler\Brofiler.h"
#include "p2Point.h"


UiItem_Bar::UiItem_Bar(SDL_Rect hitBox, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot, TypeBar type) :UiItem(hitBox, parent, pivot)
{
	assert(parent != nullptr);
	frame_bar = *section;

	if (type == TypeBar::HORIZONTAL)
	{
		SDL_Rect Rect_thumb_volume = { 646,140,51,52 };
		thumb = App->Gui->AddImage({ 157,-15,51,52 }, &Rect_thumb_volume, this, { 0,0 });
		this->type = type;
		thumb->draggable = true;
	}
	if (type == TypeBar::VERTICAL)
	{
		SDL_Rect Rect_thumb_credits = { 646,140,51,52 };
		thumb_credits = App->Gui->AddImage({ 157,-15,51,52 }, &Rect_thumb_credits, this, { 0,0 });
		this->type = type;
		thumb_credits->draggable = true;
	}


}

void UiItem_Bar::Draw()
{
	BROFILER_CATEGORY("Draw_Bar.cpp", Profiler::Color::AliceBlue)
	if (this->type == TypeBar::HORIZONTAL)
		App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pivot.x, hitBox.y - pivot.y, &frame_bar, SDL_FLIP_NONE, 0.0F);

	if (this->type == TypeBar::VERTICAL)
		App->render->Blit((SDL_Texture*)App->Gui->getTexture(), hitBox.x - pivot.x, hitBox.y - pivot.y, &frame_bar, SDL_FLIP_NONE, 0.0F, 90);
}

float UiItem_Bar::GetBarValue()
{
	if (this->type == TypeBar::HORIZONTAL)
	{
		float ipos_bar = thumb->hitBox.x + (thumb->hitBox.w / 2);
		float fixed_pos = hitBox.x + (thumb->hitBox.w / 2);
		float fpos_bar = hitBox.x + hitBox.w - (thumb->hitBox.w / 2);
		float final_pos = (ipos_bar - fixed_pos) / (fpos_bar - fixed_pos);
		LOG("FINAL POSITION OF THE THUMB IS %.2f", final_pos);
		return final_pos;
	}
}

