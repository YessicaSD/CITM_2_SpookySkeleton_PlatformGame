#ifndef UIITEM_BAR_H
#define UIITEM_BAR_H

#include "UiItem.h"
#include "p2Point.h"
#include "j1Textures.h"

class UiItem_Image;

enum TypeBar :uint
{
	VERTICAL,
	HORIZONTAL,
	MAX
};

class UiItem_Bar :public UiItem
{
	
public:
	UiItem_Bar(SDL_Rect hitBox, const SDL_Rect* section, UiItem*const parent, p2Point<int> pivot = { 0,0 }, TypeBar type = HORIZONTAL);
	~UiItem_Bar() {}
	void Draw(const float& dt) override;
	float GetBarValue();

protected:
	SDL_Rect frame_bar;
	UiItem_Image* thumb = nullptr;
	UiItem_Image* thumb_credits = nullptr;
	TypeBar type = HORIZONTAL;
};


#endif