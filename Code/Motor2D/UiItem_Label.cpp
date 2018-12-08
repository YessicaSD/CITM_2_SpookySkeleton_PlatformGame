#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"

UiItem_Label::UiItem_Label(p2Point<int> pos, p2SString text, SDL_Color color, TTF_Font * font):UiItem(pos)
{
	texture = App->font->Print(text.GetString(), color, font);
	App->tex->GetSize(texture,(uint &) HitBox.w, (uint &)HitBox.h);
	

	p2SString hoverText(text);
	hoverText += " is Hover";
	textureHover = App->font->Print(hoverText.GetString(), color, font);
	
	this->font = font;

}



void UiItem_Label::Draw()
{
	App->render->Blit(texture, HitBox.x, HitBox.y,NULL,SDL_FLIP_NONE,0.0F);
}
