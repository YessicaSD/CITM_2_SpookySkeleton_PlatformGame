#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"


#include "p2Point.h"


enum UI_STATES
{
	IDLE,
	HOVER,
	CLICK,
	MAX_STATES,
};

class UiItem
{
public:

	UI_STATES state = IDLE;
	SDL_Rect hitBox;
	p2Point<int> pivot = {0,0};
	
	UiItem(SDL_Rect hitBox, p2Point<int> pivot = {0,0}):hitBox(hitBox),pivot(pivot)
	{}

	virtual void Draw() {};
	virtual void OnClick(uint mouseButton){}



};

#endif // !ITEM_UI_H

