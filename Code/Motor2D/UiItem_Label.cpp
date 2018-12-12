#include "UiItem_Label.h"
#include "UiItem.h"

#include "j1App.h"
#include "j1Fonts.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "p2Defs.h"

UiItem_Label::UiItem_Label( p2SString text, SDL_Color color, TTF_Font * font, p2Point<int> position):UiItem()
{
	hitBox.x = position.x;
	hitBox.y = position.y;
	texture = App->font->Print(text.GetString(), color, font);
	textureHover = texture;
	App->tex->GetSize(texture,(uint &) hitBox.w, (uint &)hitBox.h);

	this->text = text;
	this->font = font;
	this->color = color;
}

bool UiItem_Label::ChangeTextureHoverOrIDLE(const p2SString * string, const SDL_Color * color, const TTF_Font * font, bool ChanegHoverTex)
{
	bool ret = false;
	assert(string != NULL || color != NULL || font != NULL);
	
	const char* txt = (string != NULL) ? string->GetString() : this->text.GetString();
	SDL_Color col = (color != NULL) ? *color : this->color;
	TTF_Font * f = (font != NULL) ? (TTF_Font *) font : this->font;

	SDL_Texture* aux = App->font->Print(txt, col, f);
	
	if (ret = (aux != nullptr) ? true : false)
	{
		SDL_Texture** tex = (ChanegHoverTex == true) ? &textureHover : &texture;
		if (textureHover != texture)
		{
			App->tex->UnLoad(*tex);
		}

		*tex = aux;
		
	}

	return ret;
	
	
}





void UiItem_Label::Draw()
{
	bool idle = (state == IDLE) ? true : false;

	if (idle)
		App->render->Blit(texture, hitBox.x, hitBox.y, NULL, SDL_FLIP_NONE, 0.0F);
	else
		App->render->Blit(textureHover, hitBox.x, hitBox.y, NULL, SDL_FLIP_NONE, 0.0F);


}
