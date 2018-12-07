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
protected:
	
	SDL_Rect frames[MAX_STATES];
	
	
public:
	UI_STATES state = IDLE;
	SDL_Rect HitBox;
	UiItem(p2Point<int> position)
	{
		HitBox.x = position.x;
		HitBox.y = position.y;
	}

	virtual void Draw();

	virtual void AddHover(const SDL_Rect* section);

	virtual void AddClick(const SDL_Rect* section);

};

#endif // !ITEM_UI_H

