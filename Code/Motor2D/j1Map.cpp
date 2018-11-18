#include "j1Map.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1ModuleEntity.h"
#include "j1Entity.h"
#include "ModuleFadeToBack.h"
#include "j1Pathfinding.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "Scene.h"
#include "Brofiler/Brofiler.h"

#include <math.h>


j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}


// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());
	for (pugi::xml_node scene_node = config.child("scenes"); scene_node; scene_node = scene_node.next_sibling("scenes"))
	{
		Scenes* scene = new Scenes();
		scene->levelnum = scene_node.attribute("lvlnum").as_uint();
		
		scene->level_tmx = scene_node.attribute("tmx").as_string();
		scene->musicPath = scene_node.attribute("music").as_string();
		level.scenes_List.add(scene);
	}
	atualSceneItem = activateScene(1);
	return ret;
}

bool j1Map::Start()
{
	App->map->Load(atualSceneItem->data->level_tmx.GetString());
	App->audio->PlayMusic(atualSceneItem->data->musicPath.GetString());
	
	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
		App->pathfinding->SetMap(w, h, data);
	
	RELEASE_ARRAY(data);

	
	return true;
}

bool j1Map::PreUpdate(float dt)
{
	BROFILER_CATEGORY("PreUpdate_Map.cpp", Profiler::Color::Salmon)
	return true;
}

bool j1Map::Update(float dt)
{
	BROFILER_CATEGORY("Update_Map.cpp", Profiler::Color::Coral)

	
	if (App->entity->entity_player!=nullptr && App->entity->entity_player->position.x >= (level.width*level.tile_width) - 2 * level.tile_width)
		{
			for (p2List_item<Scenes*>* item_scene = level.scenes_List.start; item_scene; item_scene = item_scene->next)
			{
				if (item_scene->data->levelnum == 1)
				{
					App->fade->FadeToBlack(2);
				}
				else if (item_scene->data->levelnum == 2)
				{
					App->fade->FadeToBlack(1);
				}
			}
		}
	
	return true;
}

bool j1Map::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate_Map.cpp", Profiler::Color::MediumSlateBlue)
	bool ret = true;

	if (map_loaded == false)
		return false;

	for (p2List_item<MapLayer*>* item_layer = level.layers.start; item_layer; item_layer = item_layer->next)
	{

		for (uint row = 0; row<level.height; row++)
		{
			for (uint column = 0; column<level.width; column++)
			{
				uint id = item_layer->data->dataMapLayer[Get(column, row)];
				if (id > 0)
				{
					iPoint mapPoint = MapToWorld(column, row);
					TileSet* tileset = GetTilesetFromTileId(id);
					SDL_Rect section = tileset->GetTileRect(id);
					float speed = item_layer->data->properties.Get("parallax", 0);
					App->render->Blit(tileset->texture, mapPoint.x, mapPoint.y, &section, SDL_FLIP_NONE, speed);


				}

			}

		}

	}

	

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->LoadGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;



	return ret;
}



TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* actualTile;
	for (actualTile = level.tilesets.end; id < actualTile->data->firstgid; actualTile = actualTile->prev) {}

	return actualTile->data;
}


inline iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (level.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * level.tile_width;
		ret.y = y * level.tile_height;
	}
	else if (level.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (level.tile_width * 0.5F);
		ret.y = (x + y) * (level.tile_height * 0.5F);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (level.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / level.tile_width;
		ret.y = y / level.tile_height;
	}
	else if (level.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = level.tile_width * 0.5F;
		float half_height = level.tile_height * 0.5F;
		ret.x = int((x / half_width + y / half_height) / 2) - 1;
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets----------------------------------------------------------------------------------
	
	for (p2List_item<TileSet*>* item = level.tilesets.end; item; item = item->prev)
	{
		RELEASE(item->data);
	}
	level.tilesets.clear();


	// Removed all layers----------------------------------------------------------------------------------
	for (p2List_item<MapLayer*>* Layer_item = level.layers.end; Layer_item; Layer_item = Layer_item->prev)
	{
		RELEASE(Layer_item->data);
	}
	level.layers.clear();

	//Removed Col------------------------------------------------------------------------------------------
	for (p2List_item<Object_Layer*>* Col_layer = level.collition_layers.end; Col_layer; Col_layer = Col_layer->prev)
	{
		
		for (p2List_item<Collider*>* Col_item = Col_layer->data->col.end;   Col_item;  Col_item = Col_item->prev)
		{
			if (Col_item->data != nullptr)
			{
				Col_item->data->to_delete = true;
				Col_item->data = nullptr;
			}
		}
		Col_layer->data->col.clear();
		RELEASE(Col_layer->data);
	}

	level.collition_layers.clear();

	// Clean up the pugui tree
	map_file.reset();
	
	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	BROFILER_CATEGORY("Load_Map.cpp", Profiler::Color::Red)
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		level.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	for (pugi::xml_node layer = map_file.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		ret = LoadLayer(layer, set);

		level.layers.add(set);

	}

	//Load Collision info
	for (pugi::xml_node collision = map_file.child("map").child("objectgroup"); collision && ret; collision = collision.next_sibling("objectgroup"))
	{
		Object_Layer*	coll = new Object_Layer();
		LoadCollision(collision, coll);
		level.collition_layers.add(coll);

	}

	//// Load properties  -----------------------------------------

	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", level.width, level.height);
		LOG("tile_width: %d tile_height: %d", level.tile_width, level.tile_height);

		p2List_item<TileSet*>* item = level.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = level.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		
		p2List_item<Object_Layer*>* item_coll = level.collition_layers.start;
		while (item_coll != NULL)
		{
	
			LOG("Collision ----");
			LOG("name: %s", item_coll->data->name.GetString());
			p2List_item<Collider*>* item_obj = item_coll->data->col.start;
			while (item_obj != NULL)
			{
				
				item_obj = item_obj->next;
			}
			item_coll = item_coll->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		level.width = map.attribute("width").as_uint();
		level.height = map.attribute("height").as_uint();
		level.tile_width = map.attribute("tilewidth").as_uint();
		level.tile_height = map.attribute("tileheight").as_uint();

		bool ret = false;

		pugi::xml_node data = map.child("properties");
		if (data != NULL)
		{
			pugi::xml_node prop;
			for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
			{
				Properties::Property* p = new Properties::Property();
				p->name = prop.attribute("name").as_string();
				p->value = prop.attribute("value").as_float();
				level.properties.list.add(p);
			}
		}

	
		p2SString orientation(map.attribute("orientation").as_string());
		if (orientation == "orthogonal")
		{
			level.type = MAPTYPE_ORTHOGONAL;
			
		}
		else if (orientation == "isometric")
		{
			level.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			level.type = MAPTYPE_STAGGERED;
		}
		else
		{
			level.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	for (pugi::xml_node image = tileset_node.child("image"); image != NULL; image = image.next_sibling("image"))
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_uint();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_uint();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = (set->tex_width ) / (set->tile_width );
		set->num_tiles_height = (set->tex_height ) / (set->tile_height);
		LOG("PERFECT PARSING TILESET WITH PATH: %s", image.attribute("source").as_string());
	}

	return ret;
}

bool j1Map::LoadCollision(pugi::xml_node& node, Object_Layer* object_layer)
{
	bool ret = true;
	SDL_Rect	rect;

	object_layer->name = node.attribute("name").as_string();
	LOG("%s", object_layer->name.GetString());

	for (pugi::xml_node object_node = node.child("object"); object_node != NULL; object_node = object_node.next_sibling("object"))
	{

		Collider* item_object = new Collider;
		rect.w = object_node.attribute("width").as_float();
		rect.h = object_node.attribute("height").as_float();
		rect.x = object_node.attribute("x").as_float();
		rect.y = object_node.attribute("y").as_float();

		p2SString nameObje(object_node.attribute("name").as_string());
		if (nameObje=="WALL")
			item_object = App->collision->AddCollider(rect, COLLIDER_WALL, App->map);
		

		if (nameObje == "DEATH")
			item_object = App->collision->AddCollider(rect, COLLIDER_ENEMY, App->map);
		

		if (nameObje == "RESTART")
			item_object = App->collision->AddCollider(rect, COLLIDER_RESPAWN, App->map);
		
		if (nameObje == "SPECIAL")
			item_object = App->collision->AddCollider(rect, COLLIDER_SPECIAL, App->map);
		
		if (nameObje == "ICE")
			item_object = App->collision->AddCollider(rect, COLLIDER_ICE, App->map);
	
		object_layer->col.add(item_object);

	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	LoadLayerProperties(node,layer->properties );

	pugi::xml_node layer_data = node.child("data");
	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->dataMapLayer = new uint[layer->width*layer->height];
		memset(layer->dataMapLayer, 0, layer->width*layer->height);

		int i = 0;
		for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->dataMapLayer[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

void j1Map::LoadLayerProperties(pugi::xml_node& node, Properties& properties)
{
	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_float();

			properties.list.add(p);
		}
	}

	

}


bool j1Map::Load(pugi::xml_node&nodeMap)
{
	App->scene->loadingSaveFile = true;
	App->fade->FadeToBlack(nodeMap.child("levelToLoad").attribute("num").as_int());

	return true;
}

bool j1Map::Save(pugi::xml_node& map) const
{
	map.append_child("levelToLoad").append_attribute("num").set_value(App->scene->num_thismaplvl);

	return true;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = true;
	p2List_item<MapLayer*>* item;
	item = level.layers.start;

	for (item = level.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < level.height; ++y)
		{
			for (int x = 0; x < level.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = level.width;
		height = level.height;
		ret = true;

		break;
	}

	return ret;
}

float Properties::Get(const char * value, float default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}
