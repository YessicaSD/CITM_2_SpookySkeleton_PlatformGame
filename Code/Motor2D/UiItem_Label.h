#ifndef UIITEM_LABEL_H
#define UIITEM_LABEL_H

#include "UiItem.h"
#include "p2SString.h"

#include "SDL_ttf/include/SDL_ttf.h"
#include "p2Point.h"

class UiItem_Label : public UiItem
{

	private:
		TTF_Font* font = nullptr;
		p2SString text;
		SDL_Color color;

		SDL_Texture* texture = nullptr;
		SDL_Texture* textureHover = nullptr;

	public:
		UiItem_Label( p2SString text, SDL_Color color, TTF_Font * font, p2Point<int> position, UiItem * const parent);
		bool ChangeTextureHover(const p2SString * textHover, const SDL_Color* color, const TTF_Font* font);
		void Draw(const float& dt) override;
};

#endif
