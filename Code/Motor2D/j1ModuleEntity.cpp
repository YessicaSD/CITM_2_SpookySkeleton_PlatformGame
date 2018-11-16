#include "j1App.h"
#include "j1ModuleEntity.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Entity.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Pathfinding.h"

#include "Player.h"
#include "EntityBat.h"
#include "EntityZombie.h"

ModuleEnemies::ModuleEnemies()
{
	name.create("entities");
}

ModuleEnemies::~ModuleEnemies()
{

}


bool ModuleEnemies::Awake(pugi::xml_node &node)
{
	bool ret = true;
	const char* path = node.child_value();
	LOG("%s", node.child_value());
	pugi::xml_parse_result result = enemiesFile.load_file(path);
	if (result == NULL)
	{
		LOG("ERROR ENTITIES LOADING FILE %s", result.description());
		return ret = false;
	}

	entitiesNodeDoc = enemiesFile.child("entities");
	
	return true;
}
bool ModuleEnemies::Start()
{
	bool ret = true;
	const char*	path = entitiesNodeDoc.child("player1").child("image").attribute("source").as_string();

	if ((playerTexture = App->tex->Load(path))==nullptr)
	{
		LOG("ERROR LOADING TEXTURE PLAYER");
		return ret = false;
	}
	path = entitiesNodeDoc.child("EnemiesImage").attribute("source").as_string();
	if ((entitiesTexture = App->tex->Load(path)) == nullptr)
	{
		LOG("ERROR LOADING TEXTURE ENEMIES");
		return ret = false;
	}

	LoadAnimations(entitiesNodeDoc.child("player1").child("animation"));
	LoadAnimations(entitiesNodeDoc.child("bat").child("animation"));
	LoadAnimations(entitiesNodeDoc.child("zombie").child("animation"));

	return ret;
}
bool ModuleEnemies::PreUpdate(float dt)
{
	this->dt = dt;
	p2List_item<j1Entity*>* actualEntity=nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		actualEntity->data->PreUpdate(dt);
	}
	return true;
}

bool ModuleEnemies::Update(float dt)
{
	p2List_item<j1Entity*>* actualEntity = nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		actualEntity->data->Move(dt);
	}
	return true;
}

bool ModuleEnemies::PostUpdate()
{
	p2List_item<j1Entity*>* actualEntity = nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		actualEntity->data->Draw();
	}
	return true;
}

bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");
	list_Entities.clear();
	return true;
}

void ModuleEnemies::OnCollision(Collider * c1, Collider * c2)
{
	p2List_item<j1Entity*>* actualEntity = nullptr;
	for (actualEntity = list_Entities.start; actualEntity; actualEntity = actualEntity->next)
	{
		if (actualEntity->data->collider == c1)
				actualEntity->data->OnCollision(c2);
		
	}
}

j1Entity* ModuleEnemies::AddEntity(ENEMY_TYPES type, fPoint pos)
{
	j1Entity* newEntity = nullptr;
	static_assert(UNKNOW >= 3, "code need update");
	switch (type)
	{
		case PLAYER:
			newEntity = new Player(pos, entitiesAnimation[PLAYER], playerTexture);
			entity_player = newEntity;

			break;
		case ENEMY_BAT:
			newEntity = new EntityBat(pos, entitiesAnimation[ENEMY_BAT], entitiesTexture);
		break;

		case ENEMI_ZOMBIE:
			newEntity = new EntityZombie(pos, entitiesAnimation[ENEMI_ZOMBIE], entitiesTexture);
		break;
	}
		list_Entities.add(newEntity);

	return nullptr;
}

bool ModuleEnemies::DestroyEntity(j1Entity * entity)
{
	if (entity == nullptr)
		return false;

	bool found = false;

	p2List_item<j1Entity*>* itemEntity = nullptr;	
	for (itemEntity = list_Entities.start; itemEntity != nullptr || found!= false; itemEntity = itemEntity->next)
	{
		if (itemEntity->data == entity)
			found = true;	
	}
	if (found)
	{
		list_Entities.del(itemEntity);
		return true;
	}
	return false;
}

bool ModuleEnemies::LoadAnimations(pugi::xml_node animNode) 
{
	bool ret = true;
	int numAnim = 0;
	for (pugi::xml_node thisanimNode = animNode; thisanimNode; thisanimNode = thisanimNode.next_sibling("animation"))
		++numAnim;

	Animation* anim_aux = new Animation[numAnim];

	SDL_Rect frameRect;
	int animArrayNum = 0;
	for (pugi::xml_node thisAnimNode = animNode; thisAnimNode; thisAnimNode = thisAnimNode.next_sibling("animation"))
	{
		anim_aux[animArrayNum].speed = thisAnimNode.attribute("anim_speed").as_float();
		for (pugi::xml_node frame = thisAnimNode.child("frame"); frame; frame=frame.next_sibling("frame"))
		{
			frameRect.x = frame.attribute("x").as_int();
			frameRect.y = frame.attribute("y").as_int();
			frameRect.w = frame.attribute("width").as_int();
			frameRect.h = frame.attribute("height").as_int();
		
			anim_aux[animArrayNum].PushBack(frameRect);
		}
		++animArrayNum;
	}
	entitiesAnimation.add(anim_aux);

	return ret;
}


