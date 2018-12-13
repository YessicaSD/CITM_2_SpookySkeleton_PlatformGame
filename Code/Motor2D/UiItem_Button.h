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
	UiItem_Button(SDL_Rect hitBox, const SDL_Rect * idle, UiItem* const parent, const SDL_Rect * click=nullptr, const SDL_Rect * hover = nullptr, p2Point<int> pivot = { 0,0 });
	void Draw() override;
	void OnClickUp() override;
	void OnClickDown() override;

};

#endif