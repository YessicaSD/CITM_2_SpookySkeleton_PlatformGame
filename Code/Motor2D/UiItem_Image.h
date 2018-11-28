#ifndef UIITEM_IMAGE_H
#define UIITEM_IMAGE_H
#include "itemUI.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image :public itemUi
{
public:
	SDL_Rect section = { 0,0,0,0 };

public:
	UiItem_Image(p2Point<int> pos, SDL_Texture* texture, const SDL_Rect* section) : itemUi(pos)
	{
		this->texture = texture;
		if (section == NULL)
		{
			App->tex->GetSize(texture, (uint&)this->section.w, (uint&)this->section.h);
		}
		else
		{
			this->section = *section;
		}
	}
	void Draw() override
	{
		if (texture != nullptr)
		{
			App->render->Blit(texture, rect.x, rect.y, &section, SDL_FLIP_NONE, 1.0F);
		}
	}
};
#endif // !UIITEM_IMAGE_H