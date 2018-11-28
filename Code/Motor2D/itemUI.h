#ifndef ITEM_UI_H
#define ITEM_UI_H

#include "SDL/include/SDL.h"
#include "j1App.h"
#include "j1Render.h"

#include "p2Point.h"


class itemUi
{
protected:
	SDL_Rect rect = { 0,0,0,0 };
	SDL_Texture* texture;
public:
	itemUi(p2Point<int> position)
	{
		rect.x = position.x;
		rect.y = position.y;
	}
	virtual void Draw()
	{
		if (texture != nullptr)
		{
			App->render->Blit(texture, rect.x, rect.y, NULL, SDL_FLIP_NONE, 1.0F);
		}
	}
};

#endif // !ITEM_UI_H