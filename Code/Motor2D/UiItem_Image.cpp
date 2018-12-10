#include "UiItem_Image.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Gui.h"
UiItem_Image::UiItem_Image(p2Point<int> pos, const SDL_Rect * section, p2Point<int> pivot) : UiItem(pos,pivot)
{
	HitBox.w = section->w;
	HitBox.h = section->h;
}

void UiItem_Image::Draw()
{
	App->render->Blit((SDL_Texture*)App->Gui->getTexture(), HitBox.x-pivot.x, HitBox.y-pivot.y, &frames[state], SDL_FLIP_NONE, 0.0F);

}

