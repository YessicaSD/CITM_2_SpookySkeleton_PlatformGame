#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "ModuleFadeToBack.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame


// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	
	/*if (App->player1->flPos.x >= (App->map->data.width*App->map->data.tile_width)-(3*App->map->data.tile_width))
	{
		App->fade->FadeToBlack(App->scene, App->scene2);
	}*/

	

	return true;
}

// Called each loop iteration


// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	if (App->scene->active)
	{
		App->map->CleanUp();
	}
	App->scene->Disable();
	return true;
}
