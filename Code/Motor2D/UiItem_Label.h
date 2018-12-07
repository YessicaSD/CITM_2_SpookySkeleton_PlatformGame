#ifndef UIITEM_LABEL_H
#define UIITEM_LABEL_H

#include "UiItem.h"
#include "p2SString.h"

#include "SDL_ttf/include/SDL_ttf.h"
#include "p2Point.h"

class UiItem_Label : public UiItem
{
	SDL_Texture* texture = nullptr;
	SDL_Texture* textureHover = nullptr;

public:
	TTF_Font* font=nullptr;
	p2SString text;
	SDL_Color color;
	UiItem_Label(p2Point<int> pos, p2SString text, SDL_Color color, TTF_Font * font);
	void CreateText(const char* text, SDL_Color color, TTF_Font* font);

};

#endif
