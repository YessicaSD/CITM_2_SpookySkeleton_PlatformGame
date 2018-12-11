#include "j1Gui.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"

#include "j1Fonts.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	for (uint iter =0; iter <MAX_FONTS; iter++)
	{
		arrayFonts[iter] = nullptr;
	}
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	PathTextureUI = conf.child("UITexture").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	UITexture = App->tex->Load(PathTextureUI.GetString());
	arrayFonts[BASE_FONT] = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf");
	arrayFonts[COPPERPLATE_B_I_24] = App->font->Load("fonts/CopperPlate/CopperPlate_BoldItalic.ttf",24);
	SDL_Rect sec = { 485, 829, 328, 103 };
	
	return true;
}



bool j1Gui::Update(float dt)
{
	iPoint mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	uint mouseButtonDown = App->input->GetMouseButtonDown();
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		if (mousePos.x > thisItem->data->hitBox.x-thisItem->data->pivot.x 
			&& mousePos.x < thisItem->data->hitBox.x - thisItem->data->pivot.x + thisItem->data->hitBox.w 
			&& mousePos.y>thisItem->data->hitBox.y - thisItem->data->pivot.y 
			&& mousePos.y < thisItem->data->hitBox.y - thisItem->data->pivot.y + thisItem->data->hitBox.h)
		{
			if (mouseButtonDown != 0)
			{
				thisItem->data->OnClick(mouseButtonDown);
				if (thisItem->data->state != CLICKDOWN)
					thisItem->data->state = CLICKDOWN;
			}
				
			
			else if (thisItem->data->state != HOVER)
					thisItem->data->state = HOVER;
			
			
		}
		else  if (thisItem->data->state != IDLE)
			thisItem->data->state = IDLE;


		
	}
	return true;
}
// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		thisItem->data->Draw();
		if (showUIHitBox)
		{
			SDL_Rect rect = thisItem->data->hitBox;
			if (thisItem->data->pivot != p2Point<int>(0, 0))
			{
				rect.x = rect.x - thisItem->data->pivot.x;
				rect.y = rect.y - thisItem->data->pivot.y;
			}
			
			App->render->DrawQuad(rect,255,255,255,255,false, false);
		}
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		showUIHitBox = !showUIHitBox;
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		ListItemUI.del(thisItem);
	}
	ListItemUI.clear();
	return true;
}


UiItem_Label* j1Gui::AddLabel(SDL_Rect hitBox, const char* text, SDL_Color color, TTF_Font * font)
{
	UiItem* newLabel = new UiItem_Label(hitBox, text, color, font);
	ListItemUI.add(newLabel);
	UiItem_Label* thisLabel = (UiItem_Label*) newLabel;
	return thisLabel;
}

UiItem_Image * j1Gui::AddImage(SDL_Rect hitBox, const SDL_Rect * section, p2Point<int> pivot)
{
	UiItem* newImage = new UiItem_Image(hitBox, section, pivot);
	ListItemUI.add(newImage);
	UiItem_Image* thisImage = (UiItem_Image*)newImage;
	return thisImage;
	
}

 
// const getter for atlas
 const SDL_Texture* j1Gui::getTexture() const
{
	return UITexture;
}

// class Gui ---------------------------------------------------

