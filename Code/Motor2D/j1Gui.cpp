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
	BaseFont = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf");
	AddLabel({ 0,0 }, "HOLAAAAAA", { 255,255,255,255 }, BaseFont);
	
	SDL_Rect sec = { 485, 829, 328, 103 };
	//AddImage({ 10,10 }, atlas, &sec);
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update()
{
	iPoint mousePos;
	App->input->GetMousePosition(mousePos.x, mousePos.y);
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		if (mousePos.x > thisItem->data->HitBox.x && mousePos.x<thisItem->data->HitBox.x + thisItem->data->HitBox.w && mousePos.y>thisItem->data->HitBox.y && mousePos.y < thisItem->data->HitBox.y + thisItem->data->HitBox.h)
		{
			if (App->input->GetMouseButtonDown(1)==KEY_DOWN)
			{
				
			}
			else if (App->input->GetMouseButtonDown(2) == KEY_DOWN)
			{

			}
			else
			{
				
			}
		}
		else
		{
			if (thisItem->data->state != IDLE)
			{
				thisItem->data->state = IDLE;
			}
		}
	}
	return true;
}
// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		thisItem->data->Draw();
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


UiItem_Label* j1Gui::AddLabel(p2Point<int> pos, const char* text, SDL_Color color, TTF_Font * font)
{
	UiItem* newLabel = new UiItem_Label(pos, text, color, font);
	ListItemUI.add(newLabel);
	UiItem_Label* thisLabel = (UiItem_Label*) newLabel;
	return thisLabel;
}

UiItem_Image * j1Gui::AddImage(p2Point<int> pos, const SDL_Rect * section)
{
	UiItem* newImage = new UiItem_Image(pos, section);
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

