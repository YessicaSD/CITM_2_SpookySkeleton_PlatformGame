#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"

UiItem_Label::UiItem_Label(SDL_Rect hitBox, p2SString text, SDL_Color color, TTF_Font * font):UiItem(hitBox)
{
	texture = App->font->Print(text.GetString(), color, font);
	App->tex->GetSize(texture,(uint &) hitBox.w, (uint &)hitBox.h);
	

	p2SString hoverText(text);
	hoverText += " is Hover";
	textureHover = App->font->Print(hoverText.GetString(), color, font);
	
	this->font = font;

}



void UiItem_Label::Draw()
{
	App->render->Blit(texture, hitBox.x, hitBox.y,NULL,SDL_FLIP_NONE,0.0F);
}
