#ifndef UIITEM_BUTTON_H
#define UIITEM_BUTTON_H
#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Button :public UiItem
{
protected:
	SDL_Rect frames[MAX_STATES];


public:
	SDL_Rect section = {0,0,0,0};
	
public:
	UiItem_Button(SDL_Rect hitBox, const SDL_Rect* section, p2Point<int> pivot);
	

};

#endif