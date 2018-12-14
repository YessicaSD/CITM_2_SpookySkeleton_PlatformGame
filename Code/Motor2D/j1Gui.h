#ifndef __j1GUI_H__
#define __j1GUI_H__
#include "j1Fonts.h"
#include "j1Module.h"

#include "UiItem.h"
#include "UiItem_Image.h"
#include "UiItem_Label.h"
#include "UiItem_Button.h"
#include "UiItem_Bar.h"

#define CURSOR_WIDTH 2

enum TypeFont
{
	BASE_FONT,
	COPPERPLATE_B_I_24,
	COPPERPLATE_B_I_48,
	MAX_FONTS
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
private:
	
	bool showUIHitBox = false;
	UiItem* canvas = nullptr;
public:
	TTF_Font * arrayFonts[MAX_FONTS];
	j1Gui();

	~j1Gui();

	//Override funtions from class j1Module
	bool Awake(pugi::xml_node&) override;
	bool Start() override;
	bool Update(float dt) override;
	bool PostUpdate() override;
	bool CleanUp() override;

	// Gui creation functions
	UiItem_Label* AddLabel(const char* text, SDL_Color color, TTF_Font *const font, p2Point<int> pos, UiItem *const parent);
	UiItem_Image* AddImage(SDL_Rect hitBox, const SDL_Rect* section, UiItem *const parent, p2Point<int> pivot = {0,0});
	UiItem_Button* AddButton(SDL_Rect hitBox, const SDL_Rect * idle, UiItem *const parent, const SDL_Rect * click = NULL, const SDL_Rect * hover = NULL, p2Point<int> pivot = { 0,0 });
	UiItem_Bar* AddBar(SDL_Rect hitBox, const SDL_Rect* section, UiItem *const parent, p2Point<int> pivot = { 0,0 });
	UiItem* AddEmptyElement(iPoint pos, UiItem * const parent=nullptr);
	const SDL_Texture* getTexture() const;
	p2List<UiItem*> ListItemUI;

private:

	SDL_Texture const * UITexture;
	p2SString PathTextureUI;
};

#endif // __j1GUI_H__