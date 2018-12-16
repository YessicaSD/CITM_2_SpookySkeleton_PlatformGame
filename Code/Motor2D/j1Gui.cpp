#include "j1Gui.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Scene.h"
#include "j1Fonts.h"
#include "ModuleFadeToBack.h"
#include "j1Map.h"
#include "Brofiler\Brofiler.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
	
	canvas = new UiItem({ 0,0,0,0 }, NULL);
	ListItemUI.add(canvas);

	mapOfFuntions.PushBack("FadeToScene", FadeToScene);
	mapOfFuntions.PushBack("ExitGame", ExitGame);
	mapOfFuntions.PushBack("GoToSetting", GoToSetting);
	mapOfFuntions.PushBack("GoBackToMenu", GoBackToMenu);
	mapOfFuntions.PushBack("LoadGame", LoadGame);
}

// Destructor
j1Gui::~j1Gui()
{

}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	PathTextureUI = conf.child("UITexture").attribute("file").as_string("");
	click_fx = App->audio->LoadFx("audio/fx/click2 .wav");
	clicked = true;

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
	BROFILER_CATEGORY("Update_Gui.cpp", Profiler::Color::Coral)
	this->dt = dt;
	iPoint mousePos;
	App->input->GetMousePosition(mousePos);
	uint mouseButtonDown = App->input->GetMouseButtonDown();
	p2List<UiItem*> listUpdate;
	canvas->returnChildList(listUpdate);

	for (p2List_item<UiItem*>* thisItem = listUpdate.start; thisItem; thisItem = thisItem->next)
	{
		if (thisItem->data->draggable && thisItem->data->mouseButtonDown != 0)
		{
			iPoint mouseMotion;
			App->input->GetMouseMotion(mouseMotion);
			if (thisItem->data->HaveParent())
			{
				thisItem->data->AddToPos(mouseMotion);
			}
		}

		if (mousePos.x > thisItem->data->hitBox.x-thisItem->data->pivot.x 
			&& mousePos.x < thisItem->data->hitBox.x - thisItem->data->pivot.x + thisItem->data->hitBox.w 
			&& mousePos.y>thisItem->data->hitBox.y - thisItem->data->pivot.y 
			&& mousePos.y < thisItem->data->hitBox.y - thisItem->data->pivot.y + thisItem->data->hitBox.h)
		{
			if (thisItem->data->state != CLICK && mouseButtonDown != 0)
			{
				App->audio->PlayFx(click_fx);
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
			
			
		}
		else  if (thisItem->data->state != IDLE)
			thisItem->data->state = IDLE;
		if (App->input->GetMouseButtonState(thisItem->data->mouseButtonDown) == KEY_UP || App->input->GetMouseButtonState(thisItem->data->mouseButtonDown) == KEY_IDLE)
			thisItem->data->mouseButtonDown = 0;

		
	}
	return true;
}
// Called after all Updates

bool j1Gui::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_Gui.cpp", Profiler::Color::MediumSlateBlue)
	if (canvas->enable)
	{
		canvas->DrawChildrens(dt);
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
		delete thisItem->data;
	}
	ListItemUI.clear();
	canvas = nullptr;
	

	return true;
}


UiItem_Label* j1Gui::AddLabel(const char* text, SDL_Color color, TTF_Font *const font, p2Point<int> pos, UiItem *const parent = NULL)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = new UiItem_Label(text, color, font, pos, canvas);
	else
		newUIItem = new UiItem_Label(text, color, font, pos, parent);

	ListItemUI.add(newUIItem);
	return (UiItem_Label*) newUIItem;

}

UiItem_Image * j1Gui::AddImage(SDL_Rect hitBox, const SDL_Rect * section, UiItem *const parent = NULL, p2Point<int> pivot)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = new UiItem_Image(hitBox, section, canvas, pivot);
	else
		newUIItem = new UiItem_Image(hitBox, section, parent, pivot);

	ListItemUI.add(newUIItem);

	return (UiItem_Image*)newUIItem;
	
}

UiItem_Button * j1Gui::AddButton(SDL_Rect hitBox, const SDL_Rect* idle, p2SString& funtionName, bool Down, UiItem * const parent, const SDL_Rect * click, const SDL_Rect * hover, p2Point<int> pivot)
{
	UiItem* newUIItem = nullptr;
	if(parent == NULL)
		newUIItem = new UiItem_Button(hitBox, idle, canvas, click, hover, pivot);
	else
		newUIItem = new UiItem_Button(hitBox, idle, parent, click, hover, pivot);

	ListItemUI.add(newUIItem);
	UiItem_Button* button = (UiItem_Button*)newUIItem;
	button->AddFuntion(funtionName, Down);
	return (UiItem_Button*)newUIItem;
}

UiItem_Bar * j1Gui::AddBar(SDL_Rect hitBox, const SDL_Rect* section, UiItem *const parent, p2Point<int> pivot )
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = new UiItem_Bar(hitBox, section, canvas, pivot);
	else
		newUIItem = new UiItem_Bar(hitBox, section, parent, pivot);

	ListItemUI.add(newUIItem);
	return (UiItem_Bar*)newUIItem;
}

UiItem* j1Gui::AddEmptyElement(iPoint pos, UiItem * const parent)
{
	UiItem* newUIItem = nullptr;
	if (parent == NULL)
		newUIItem = new UiItem(pos, canvas);
	else
		newUIItem = new UiItem(pos, parent);

	ListItemUI.add(newUIItem);
	return newUIItem;
}

 
// const getter for atlas
 const SDL_Texture* j1Gui::getTexture() const
{
	return UITexture;
}

 //funtions----------------------------------------------
 void FadeToScene()
 {
	 j1Module* thisModule = (j1Module*)App->pathfinding;
	 thisModule->Enable();
	 thisModule->active = true;
	
	 App->scene->state = SceneState::GAME;
	 App->fade->FadeToBlack(1);
	

 }
 void ExitGame()
 {
	 App->scene->exitGame = true;
 }
 void GoToSetting()
 {
	 App->scene->startMenupanel->enable = false;
	 App->scene->settingPanel->enable = true;
 }

 void GoBackToMenu() {
	 App->scene->startMenupanel->enable = true;
	 App->scene->settingPanel->enable = false;
 }
 void LoadGame()
 {
	 App->scene->state = SceneState::GAME;
	 j1Module* thisModule = (j1Module*)App->pathfinding;
	 thisModule->Enable();
	 thisModule->active = true;
	 App->map->active = true;

	 App->LoadGame();
	 
 }