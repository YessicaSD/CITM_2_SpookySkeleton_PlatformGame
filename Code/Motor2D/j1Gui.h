#ifndef __j1GUI_H__
#define __j1GUI_H__
#include "j1Fonts.h"
#include "j1Module.h"

#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"

#define CURSOR_WIDTH 2

// ---------------------------------------------------
class j1Gui : public j1Module
{
private:
	TTF_Font * BaseFont = nullptr;
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update();
	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	

	
	// Gui creation functions
	UiItem_Label* AddLabel(p2Point<int> pos, const char* text, SDL_Color color, TTF_Font* font);
	UiItem_Image* AddImage(p2Point<int> pos, const SDL_Rect* section);

	SDL_Texture* getTexture() const;
	p2List<UiItem*> ListItemUI;

private:

	SDL_Texture* UITexture;
	p2SString PathTextureUI;
};

#endif // __j1GUI_H__