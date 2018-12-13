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
	ListItemUI.add(canvas);
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
	arrayFonts[BASE_FONT] = App->font->Load("fonts/open_sans/OpenSans-Bold.ttf");
	arrayFonts[COPPERPLATE_B_I_48] = App->font->Load("fonts/CopperPlate/CopperPlate_BoldItalic.ttf", 48);
	arrayFonts[COPPERPLATE_B_I_24] = App->font->Load("fonts/CopperPlate/CopperPlate_BoldItalic.ttf", 24);
	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	UITexture = App->tex->Load(PathTextureUI.GetString());
	
	SDL_Rect sec = { 485, 829, 328, 103 };
	
	return true;
}



bool j1Gui::Update(float dt)
{
	iPoint mousePos;
	App->input->GetMousePosition(mousePos);
	uint mouseButtonDown = App->input->GetMouseButtonDown();
	for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
	{
		if (mousePos.x > thisItem->data->hitBox.x-thisItem->data->pivot.x 
			&& mousePos.x < thisItem->data->hitBox.x - thisItem->data->pivot.x + thisItem->data->hitBox.w 
			&& mousePos.y>thisItem->data->hitBox.y - thisItem->data->pivot.y 
			&& mousePos.y < thisItem->data->hitBox.y - thisItem->data->pivot.y + thisItem->data->hitBox.h)
		{
			if (thisItem->data->state != CLICK && mouseButtonDown != 0)
			{
				thisItem->data->mouseButtonDown = mouseButtonDown;
				thisItem->data->OnClickDown();
				thisItem->data->state = CLICK;
			}

			if (thisItem->data->state == CLICK && App->input->GetMouseButtonState(thisItem->data->mouseButtonDown) == KEY_UP)
			{
				thisItem->data->OnClickUp();
				thisItem->data->state = HOVER;
			}
			
			else if (thisItem->data->state == IDLE)
					thisItem->data->state = HOVER;
			
			if (thisItem->data->draggable && thisItem->data->state==CLICK )
			{
				
				iPoint mouseMotion;
				App->input->GetMouseMotion(mouseMotion);
				LOG("I'M IN!! X: %i Y: %i",mouseMotion.x, mouseMotion.y);
				thisItem->data->AddToPos(mouseMotion);
				
			}
		}
		else  if (thisItem->data->state != IDLE)
			thisItem->data->state = IDLE;
		

		
	}
	return true;
}
// Called after all Updates

bool j1Gui::PostUpdate()
{
	if (canvas->enable)
	{
		canvas->DrawChildrens();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		for (p2List_item<UiItem*>* thisItem = ListItemUI.start; thisItem; thisItem = thisItem->next)
		{
			thisItem->data->showHitBox = !thisItem->data->showHitBox;
		}
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


UiItem_Label* j1Gui::AddLabel(const char* text, SDL_Color color, TTF_Font * font, p2Point<int> pos, UiItem * parent)
{
	UiItem* newUIItem = new UiItem_Label( text, color, font, pos);
	ListItemUI.add(newUIItem);
	if (parent == NULL)
	{
		newUIItem->AddParent(canvas);
		
	}
	else
	{
		newUIItem->AddParent(parent);
	}

	return (UiItem_Label*) newUIItem;

}

UiItem_Image * j1Gui::AddImage(SDL_Rect hitBox, const SDL_Rect * section, p2Point<int> pivot, UiItem * parent)
{
	UiItem* newUIItem = new UiItem_Image(hitBox, section, pivot);
	ListItemUI.add(newUIItem);

	if (parent == NULL)
	{
		newUIItem->AddParent(canvas);

	}
	else
	{
		newUIItem->AddParent(parent);
	}


	return (UiItem_Image*)newUIItem;
	
}

UiItem_Button * j1Gui::AddButton(SDL_Rect hitBox, const SDL_Rect* idle, const SDL_Rect * click, const SDL_Rect * hover, p2Point<int> pivot, UiItem * parent)
{
	UiItem* newUIItem = new UiItem_Button(hitBox, idle, click, hover, pivot);
	ListItemUI.add(newUIItem);
	if (parent == NULL)
	{
		newUIItem->AddParent(canvas);

	}
	else
	{
		newUIItem->AddParent(parent);
	}
	return (UiItem_Button*)newUIItem;
}

 
// const getter for atlas
 const SDL_Texture* j1Gui::getTexture() const
{
	return UITexture;
}

// class Gui ---------------------------------------------------

