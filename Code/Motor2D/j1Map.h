#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Collider;
struct Object
{
	int			obj_id;
	Collider*   colWall=nullptr;
	~Object()
	{
		delete[] colWall;
	}
	
};

struct Object_Layer
{
	p2SString			name;
	p2List<Object*>		object;
};

struct MapLayer
{
	p2SString name="";
	uint width=0u;
	uint height=0u;
	uint* tiledata=nullptr;
	~MapLayer()
	{
		if (tiledata != nullptr)
		{
			delete tiledata;
		}
	}
};


// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name ="";
	uint					firstgid=0;
	uint					margin=0;
	uint					spacing=0;
	uint					tile_width=0;
	uint					tile_height=0;
	SDL_Texture*		texture=nullptr;
	uint					tex_width=0;
	uint					tex_height=0;
	uint					num_tiles_width=0;
	uint					num_tiles_height=0;
	uint					offset_x=0;
	uint					offset_y=0;

	SDL_Rect Rectfind(uint tileId)
	{
			tileId - 1;
			SDL_Rect rect;
			rect.w= tex_width;
			rect.h = tex_height;
			rect.x = margin + ((rect.w + spacing) * (tileId % num_tiles_width));
			rect.y = margin + ((rect.h + spacing) * (tileId / num_tiles_width));
			return rect;
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
	uint					width;
	uint					height;
	uint					tile_width;
	uint					tile_height;
	/*SDL_Color			background_color;*/
	MapTypes			type;
	p2List<TileSet*>tilesets;
	p2List<MapLayer*> layers;
	p2List<Object_Layer*>collisions;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	MapData data;
	float gravity=0.25;
	inline uint Get(int x, int y) const
	{return  (y * data.width + x);}

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);
	iPoint MapToWorld(int x, int y) const;
	void OnCollision(Collider*, Collider*);

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
	



};



#endif // __j1MAP_H__