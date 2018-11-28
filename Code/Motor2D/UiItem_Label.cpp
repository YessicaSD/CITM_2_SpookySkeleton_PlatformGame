#include "UiItem_Label.h"
#include "j1App.h"
#include "j1Font.h"
UiItem_Label::UiItem_Label(p2Point<int> pos, const char* text, SDL_Color color, TTF_Font * font) :itemUi(pos)
{
	CreateText(text, color, font);
}

void UiItem_Label::CreateText(const char* text, SDL_Color color, TTF_Font* font)
{
	texture = App->font->Print(text, color, font);
}