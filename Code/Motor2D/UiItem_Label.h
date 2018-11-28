#ifndef UIITEM_LABEL_H
#define UIITEM_LABEL_H
#include "itemUI.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "p2SString.h"
#include "p2Point.h"
class UiItem_Label : public itemUi
{
public:
	TTF_Font* font = nullptr;
	p2SString text;
	SDL_Color color;
	UiItem_Label(p2Point<int> pos, const char* text, SDL_Color color, TTF_Font * font);
	void CreateText(const char* text, SDL_Color color, TTF_Font* font);

};

#endif