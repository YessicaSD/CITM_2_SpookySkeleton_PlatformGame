#ifndef ITEM_UI_H
#define ITEM_UI_H
#include "SDL/include/SDL.h"


#include "p2Point.h"
#include "p2DynArray.h"

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
	uint mouseButtonDown = 0;
	UiItem* parent = nullptr;
	p2DynArray<UiItem*> childs;
	UiItem(SDL_Rect hitBox, p2Point<int> pivot = {0,0}):hitBox(hitBox),pivot(pivot)
	{}

	virtual void Draw() {};
	virtual void OnClickDown(){}
	virtual void OnClickUp(){}


};

#endif // !ITEM_UI_H

