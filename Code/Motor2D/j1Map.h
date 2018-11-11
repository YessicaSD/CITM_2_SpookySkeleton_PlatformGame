#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Textures.h"

struct Collider;
struct SceneProp
{
	fPoint PlayerPos;
	fPoint CameraPos;
};
struct Scenes
{
	bool active=false;
	uint levelnum=0;
	p2SString level_tmx;
	p2SString musicPath;
	
};


struct Object_Layer
{
	p2SString			name;
	int					special_coll;
	p2List<Collider*>		col;
	~Object_Layer()
	{
		for (p2List_item<Collider*>* colItem = col.end; colItem; colItem = colItem->prev )
		{
			colItem->data->to_delete = true;
			RELEASE(colItem->data);
		}

	}

};

struct MapLayer
{
	p2SString name = "";
	uint width = 0u;
	uint height = 0u;
	float parallax_velocity = 0.0f;
	uint* tiledata = nullptr;
	~MapLayer()
	{
		if (tiledata != nullptr)
		{
			delete tiledata;
			tiledata = nullptr;
		}
	}
};


// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name = "";
	uint					firstgid = 0;
	uint					margin = 0;
	uint					spacing = 0;
	uint					tile_width = 0;
	uint					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	uint					tex_width = 0;
	uint					tex_height = 0;
	uint					num_tiles_width = 0;
	uint					num_tiles_height = 0;



	~TileSet()
	{
		if (texture != nullptr)
		{
			App->tex->UnLoad(texture);
		}
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	float gravity = 10.0f;
	uint					width;
	uint					height;
	uint					tile_width;
	uint					tile_height;
	MapTypes				type;
	p2List<TileSet*>		tilesets;
	p2List<MapLayer*>		layers;
	p2List<Object_Layer*>   collition_layers;
	p2List<Scenes*>			scenes_List;
	SceneProp SceneProperties;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
private:
	p2List_item<Scenes*>* atualSceneItem=nullptr;
public:

	MapData data;
	uint SavedLevel = 1;
	uint num_thismaplvl = 1;

	inline uint Get(int x, int y) const
	{
		return  (y * data.width + x);
	}

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	// Called each loop iteration
	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	inline iPoint MapToWorld(int x, int y) const;
	void OnCollision(Collider*, Collider*);

	fPoint returnPlayerPos()const
	{
		return data.SceneProperties.PlayerPos;
	}
	fPoint returnCameraPos()const
	{
		return data.SceneProperties.CameraPos;
	}

	p2List_item<Scenes*>* activateScene(uint lvlnum)
	{
		p2List_item<Scenes*>* item_scene;
		
		for(item_scene= data.scenes_List.start; item_scene; item_scene= item_scene->next   )
			{
			item_scene->data->active = false;
		}
		for (item_scene = data.scenes_List.start; item_scene->data->levelnum!=lvlnum && item_scene !=nullptr ; item_scene = item_scene->next)
		{	}
		item_scene->data->active = true;
		atualSceneItem = item_scene;
		return item_scene;
	}
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

	uint i = 0;
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadCollision(pugi::xml_node& coll_node, Object_Layer* collision);
	TileSet* GetTilesetFromTileId(int id) const;
	void LoadProperties(pugi::xml_node& node);


};



#endif // __j1MAP_H__