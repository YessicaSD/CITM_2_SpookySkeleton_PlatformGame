#include "j1Map.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"

#include "j1Render.h"
#include "j1Textures.h"

#include "j1Collision.h"
#include "j1Player.h"
#include "j1ModuleEntity.h"
#include "EntityBat.h"
#include "EntityZombie.h"
#include "ModuleFadeToBack.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Audio.h"
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
		data.scenes_List.add(scene);
	}
	atualSceneItem = activateScene(1);
	return ret;
}

bool j1Map::Start()
{
	App->map->Load(atualSceneItem->data->level_tmx.GetString());
	App->audio->PlayMusic(atualSceneItem->data->musicPath.GetString());
	App->player1->Enable();
	App->entity->Enable();
	debug_tex= App->tex->Load("maps/path2.png");
	return true;
}

bool j1Map::PreUpdate(float dt)
{
	
	return true;
}

bool j1Map::Update(float dt)
{
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
		App->fade->FadeToBlack(num_thismaplvl);
	}
	if (App->input->GetKey(SDL_SCANCODE_F8))
	{
		App->fade->FadeToBlack(2);
		
	}
	
	if (App->player1->flPos.x >= (data.width*data.tile_width)-2*data.tile_width)
	{
		for (p2List_item<Scenes*>* item_scene = data.scenes_List.start;item_scene;item_scene=item_scene->next)
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
	bool ret = true;

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

bool j1Map::Draw(float dt)
{
	if (map_loaded == false)
		return false;


	for (p2List_item<MapLayer*>* item_layer = data.layers.start; item_layer; item_layer = item_layer->next)
	{

		for (uint row = 0; row<data.height; row++)
		{
			for (uint column = 0; column<data.width; column++)
			{
				uint id = item_layer->data->tiledata[Get(column, row)];
				if (id > 0 )
				{
					iPoint mapPoint = MapToWorld(column, row);
					
						TileSet* tileset = GetTilesetFromTileId(id);
						SDL_Rect section = tileset->GetTileRect(id);
						App->render->Blit(tileset->texture, mapPoint.x, mapPoint.y, &section, SDL_FLIP_NONE, item_layer->data->parallax_velocity);
				
				}

			}

		}

	}
	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	return true;
}
TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* actualTile;
	for (actualTile = data.tilesets.end; id < actualTile->data->firstgid; actualTile = actualTile->prev) {}

	return actualTile->data;
}


inline iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
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
	App->player1->Disable();
	App->entity->Disable();
	for (p2List_item<TileSet*>* item = data.tilesets.end; item; item = item->prev)
	{
		
		RELEASE(item->data);
	}
	data.tilesets.clear();


	// Removed all layers----------------------------------------------------------------------------------
	for (p2List_item<MapLayer*>* Layer_item = data.layers.end; Layer_item; Layer_item = Layer_item->prev)
	{
		RELEASE(Layer_item->data);
	}
	data.layers.clear();

	//Removed Col------------------------------------------------------------------------------------------
	for (p2List_item<Object_Layer*>* Col_layer = data.collition_layers.end; Col_layer; Col_layer = Col_layer->prev)
	{
		
		for (p2List_item<Collider*>* Col_item = Col_layer->data->col.end;   Col_item;  Col_item = Col_item->prev)
		{

			Col_item->data->to_delete = true;
			Col_item->data = nullptr;

		}
		Col_layer->data->col.clear();
		RELEASE(Col_layer->data);
	}

	data.collition_layers.clear();

	// Clean up the pugui tree
	map_file.reset();
	
	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
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

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	for (pugi::xml_node layer = map_file.child("map").child("layer"); layer; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		ret = LoadLayer(layer, set);

		data.layers.add(set);

	}

	//Load Collision info
	for (pugi::xml_node collision = map_file.child("map").child("objectgroup"); collision && ret; collision = collision.next_sibling("objectgroup"))
	{
		Object_Layer*	coll = new Object_Layer();
		ret = LoadCollision(collision, coll);
		if (ret == true)
		{
			data.collition_layers.add(coll);
			
		}
	}

	// Load properties  -----------------------------------------
		LoadProperties(map_file.child("map").child("properties").child("property"));
	
	if (ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
		
		p2List_item<Object_Layer*>* item_coll = data.collition_layers.start;
		while (item_coll != NULL)
		{
			
			//Collision* c = item_coll->data;
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
		data.width = map.attribute("width").as_uint();
		data.height = map.attribute("height").as_uint();
		data.tile_width = map.attribute("tilewidth").as_uint();
		data.tile_height = map.attribute("tileheight").as_uint();
		p2SString nameProperty;
		for (pugi::xml_node nodeProperties = map.child("properties").child("property"); nodeProperties; nodeProperties = nodeProperties.next_sibling("property"))
		{
			nameProperty = nodeProperties.attribute("name").as_string();

			//Load players's initial position
			if (nameProperty == "PlayerPos_x")
			{
				App->player1->flPos.x = nodeProperties.attribute("value").as_float();

			}
			if (nameProperty == "PlayerPos_y")
			{
				App->player1->flPos.y = nodeProperties.attribute("value").as_float();

			}

			//Load bat's initial position
			if (nameProperty == "BatPos_x")
			{
				App->bat->Bat_Pos.x = nodeProperties.attribute("value").as_float();

			}

			if (nameProperty == "BatPos_y")
			{
				App->bat->Bat_Pos.y = nodeProperties.attribute("value").as_float();

			}
			//Load zombie's initial position
			if (nameProperty == "ZombiePos_x")
			{
				App->zombie->Zombie_Pos.x = nodeProperties.attribute("value").as_float();

			}

			if (nameProperty == "ZombiePos_y")
			{
				App->zombie->Zombie_Pos.y = nodeProperties.attribute("value").as_float();

			}
			
		}
		
	
		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
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
	pugi::xml_node offset = tileset_node.child("tileoffset");

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
	object_layer->special_coll = node.attribute("gravity_change").as_int();
	LOG("%s", object_layer->name.GetString());

	for (pugi::xml_node object_node = node.child("object"); object_node != NULL; object_node = object_node.next_sibling("object"))
	{

		Collider* item_object = new Collider;
		rect.w = object_node.attribute("width").as_float();
		rect.h = object_node.attribute("height").as_float();
		rect.x = object_node.attribute("x").as_float();
		rect.y = object_node.attribute("y").as_float();

		if (object_layer->name == "Wall")
		{
			item_object = App->collision->AddCollider(rect, COLLIDER_WALL, App->map);
		}

		if (object_layer->name == "Death")
		{
			item_object = App->collision->AddCollider(rect, COLLIDER_ENEMY, App->map);
		}

		if (object_layer->name == "RestartLevel")
		{
			item_object = App->collision->AddCollider(rect, COLLIDER_RESPAWN, App->map);
		}
		if (object_layer->name == "Special")
		{
			item_object = App->collision->AddCollider(rect, COLLIDER_SPECIAL, App->map);
		}
		object_layer->col.add(item_object);

	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->parallax_velocity = node.child("properties").child("property").attribute("value").as_float();
	layer->tiledata = new uint[layer->width*layer->height];
	memset(layer->tiledata, 0u, sizeof(uint)*layer->height*layer->width);

	int i = 0;
	for (pugi::xml_node tileset = node.child("data").child("tile"); tileset; tileset = tileset.next_sibling("tile"))
	{

		layer->tiledata[i] = tileset.attribute("gid").as_uint();

		LOG("%u", layer->tiledata[i]);
		++i;
	}
	return true;
}

void j1Map::LoadProperties(pugi::xml_node& node)
{
	p2SString nameProperty;
	for (pugi::xml_node& nodeProperties = node; node; nodeProperties = node.next_sibling("property"))
	{
		nameProperty = nodeProperties.attribute("name").as_string();
		if (nameProperty == "PlayerPos_x")
			data.SceneProperties.PlayerPos.x = node.attribute("value").as_float();

		if (nameProperty == "PlayerPos_y")
			data.SceneProperties.PlayerPos.y = node.attribute("value").as_float();


		if (nameProperty == "Distant_to_camera_from_player_x")
		{
			data.SceneProperties.CameraPos.x = node.attribute("value").as_float();


		}
		if (nameProperty == "Distant_to_camera_from_player_y")
		{
			data.SceneProperties.CameraPos.y = node.attribute("value").as_float();

		}
	}

}
void j1Map::OnCollision(Collider* c1, Collider* c2)
{}

bool j1Map::Load(pugi::xml_node&nodeMap)
{
	SavedLevel = nodeMap.child("lvl").attribute("num").as_uint();

	return true;
}

bool j1Map::Save(pugi::xml_node& map) const
{
	LOG("Saved level %i", num_thismaplvl);
	pugi::xml_node play = map.append_child("lvl");
	play.append_attribute("num").set_value(num_thismaplvl);
	
	return true;
}
