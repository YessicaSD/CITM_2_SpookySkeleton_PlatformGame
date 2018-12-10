#ifndef UIITEM_IMAGE_H
#define UIITEM_IMAGE_H

	#include "UiItem.h"
	#include "p2Point.h"
	#include "j1Textures.h"
	
	class UiItem_Image : public UiItem
	{		
		SDL_Rect frames[MAX_STATES];
			public:
				UiItem_Image(p2Point<int> pos, const SDL_Rect* section, p2Point<int> pivot = {0,0});

				void Draw() override;
	};
#endif // !UIITEM_IMAGE_H

