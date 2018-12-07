#include "UiItem_Label.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "UiItem.h"

UiItem_Label::UiItem_Label(p2Point<int> pos, p2SString text, SDL_Color color, TTF_Font * font):UiItem(pos)
{
	CreateText(text.GetString(),color, font);
	if(texture!=nullptr)
		App->tex->GetSize(texture, (uint&)frames[IDLE].w, (uint&)frames[IDLE].h);
	

	p2SString hoverText(text);
	hoverText += "is Hover";
	textureHover = App->font->Print(hoverText.GetString(), color, font);
	App->tex->GetSize(textureHover, (uint&)frames[HOVER].w, (uint&)frames[HOVER].h);
	this->font = font;

}

void UiItem_Label::CreateText(const char* text, SDL_Color color, TTF_Font* font)
{
	texture = App->font->Print(text, color, font);

}
