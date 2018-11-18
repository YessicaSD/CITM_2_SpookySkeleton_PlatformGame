#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "Scene.h"

#include "ModuleFadeToBack.h"
#include "Brofiler\Brofiler.h"
j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{
}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;
	const char* path = node.child("idle").child_value();
	
	pugi::xml_parse_result result = sceneFile.load_file(path);
	if (result == NULL)
	{
		LOG("ERROR ENTITIES LOADING FILE %s", result.description());
		return ret = false;
	}

	sceneNode = sceneFile.child("scene");

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{

	//Pick level node-----------------------------------------
	pugi::xml_node entitiesNode;
	if (loadingSaveFile == true)
	{
		/*if(saveNode!=nullptr)
			entitiesNode = *saveNode;
		LOG("%s", saveNode->name());*/
		loadingSaveFile = false;
	}
	else
	{
		pugi::xml_node levelNode = sceneNode.child("level");
		if (levelNode == NULL)
		{
			LOG("ERROR ENTITIES LOADING FILE");
			return false;
		}

		for (uint i = 1; i < num_thismaplvl; ++i)
		{
			levelNode = levelNode.next_sibling("level");
		}
		//Load enemies -----------------------------------------
		entitiesNode = levelNode.child("entities");
		LoadEntities(entitiesNode);
	}
	uint numEnemies = entitiesArrayInfo.Count();
	if (numEnemies > 0)
	{
		for (uint i = 0; i < numEnemies; ++i)
		{
			App->entity->AddEntity(entitiesArrayInfo[i]);
		}
	}
	
	
	if (entitiesNode == nullptr)
		return false;

	return true;
}
void j1Scene::LoadEntities(const pugi::xml_node& entitiesNode)
{
	entitiesArrayInfo.Clear();
	//players---------------------------
	for (pugi::xml_node playerNode = entitiesNode.child("player"); playerNode !=NULL; playerNode = playerNode.next_sibling("player"))
	{
		entitiesArrayInfo.PushBack(EntitiesInfo(PLAYER, { playerNode.attribute("x").as_float(),playerNode.attribute("y").as_float() }));
	}
	//bats-----------------------------
	for (pugi::xml_node entityNode = entitiesNode.child("bat"); entityNode; entityNode = entityNode.next_sibling("bat"))
	{
		entitiesArrayInfo.PushBack(EntitiesInfo (ENEMY_BAT, { entityNode.attribute("x").as_float(),entityNode.attribute("y").as_float() }));
		
	}
	//zombies---------------------------
	for (pugi::xml_node entityNode = entitiesNode.child("zombie"); entityNode; entityNode = entityNode.next_sibling("zombie"))
	{
		entitiesArrayInfo.PushBack(EntitiesInfo (ENEMI_ZOMBIE, { entityNode.attribute("x").as_float(),entityNode.attribute("y").as_float() }));
	}
}
// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Scene.cpp", Profiler::Color::Salmon)
	return true;
}

bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Update_Scene.cpp", Profiler::Color::Coral)
	//Load and save game---------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	//Camera controls -----------------------------------------------------
	if (App->input->GetKey(SDL_SCANCODE_KP_8) == KEY_REPEAT)
		App->render->camera.y -= 5;

	if (App->input->GetKey(SDL_SCANCODE_KP_5) == KEY_REPEAT)
		App->render->camera.y += 5;

	if (App->input->GetKey(SDL_SCANCODE_KP_4) == KEY_REPEAT)
		App->render->camera.x -= 5;

	if (App->input->GetKey(SDL_SCANCODE_KP_6) == KEY_REPEAT)
		App->render->camera.x += 5;

	if (App->input->GetKey(SDL_SCANCODE_F1))
	{
		App->fade->FadeToBlack(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_F2))
	{
		App->fade->FadeToBlack(App->scene->num_thismaplvl);
	}
	if (App->input->GetKey(SDL_SCANCODE_F8))
	{
		App->fade->FadeToBlack(2);

	}
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");
	App->entity->DestroyAllEntities();

	return true;
}

bool j1Scene::Load(pugi::xml_node& node)
{
	LoadEntities(node.child("level").child("entities"));
	return true;
}

bool j1Scene::Save(pugi::xml_node & node) const
{	
	
	pugi::xml_node nodeEntities = node.append_child("level").append_child("entities");
	
	for (p2List_item<j1Entity*>* enemiesIterator = App->entity->list_Entities.start; enemiesIterator; enemiesIterator = enemiesIterator->next)
	{
		pugi::xml_node entity;
		switch (enemiesIterator->data->type)
		{
		case PLAYER:
			entity = nodeEntities.append_child("player");
			entity.append_attribute("x").set_value((int)enemiesIterator->data->position.x);
			entity.append_attribute("y").set_value((int)enemiesIterator->data->position.y);
			break;
		case ENEMY_BAT:
			entity = nodeEntities.append_child("bat");
			entity.append_attribute("x").set_value((int)enemiesIterator->data->position.x);
			entity.append_attribute("y").set_value((int)enemiesIterator->data->position.y);
			break;
		case ENEMI_ZOMBIE:
			entity = nodeEntities.append_child("zombie");
			entity.append_attribute("x").set_value((int)enemiesIterator->data->position.x);
			entity.append_attribute("y").set_value((int)enemiesIterator->data->position.y);
			break;
		default:
			break;
		}
	}
	return true;
}
