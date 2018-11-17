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
#include "j1ModuleEntity.h"

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
	const char* path = node.child_value();
	LOG("%s", node.child_value());
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
	pugi::xml_node levelNode = sceneNode.child("level");
	for (uint i = 1; i < App->map->num_thismaplvl; ++i)
	{
		levelNode = levelNode.next_sibling("level");
	}
	//Load enemies -----------------------------------------
	pugi::xml_node entitiesNode = levelNode.child("entities");
	if(entitiesNode==nullptr)
		return false;
	//players---------------------------
	for (pugi::xml_node playerNode = entitiesNode.child("players").child("player"); playerNode; playerNode=playerNode.next_sibling("player"))
	{
		App->entity->AddEntity(PLAYER, { playerNode.attribute("x").as_float(),playerNode.attribute("y").as_float() });
				}
	//bats-----------------------------
	for (pugi::xml_node batNode = entitiesNode.child("bats").child("bat"); batNode; batNode = batNode.next_sibling("bat"))
	{
		App->entity->AddEntity(ENEMY_BAT, { batNode.attribute("x").as_float(),batNode.attribute("y").as_float() });
	}
	//zombies---------------------------
	for (pugi::xml_node zombieNode = entitiesNode.child("zombies").child("zombie"); zombieNode; zombieNode = zombieNode.next_sibling("zombie"))
	{
		App->entity->AddEntity(ENEMI_ZOMBIE, { zombieNode.attribute("x").as_float(),zombieNode.attribute("y").as_float() });
	}
	
	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate(float dt)
{
	return true;
}

bool j1Scene::Update(float dt)
{
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
