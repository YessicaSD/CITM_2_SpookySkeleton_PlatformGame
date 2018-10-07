#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

// TODO 1: Create a struct for the map layer
// ----------------------------------------------------
struct Object
{
	int			obj_id;
	float		x;
	float       y;
	float		width;
	float		height;
};

struct Collision
{
	p2SString			name;
	SDL_Rect			rect;
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

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture=nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

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
	p2List<TileSet*>	tilesets;
	// TODO 2: Add a list/array of layers to the map!
	p2List<MapLayer*> layers;
	p2List<Collision*>	collisions;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

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

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single layer
    bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadCollision(pugi::xml_node& coll_node, Collision* collision);
	

public:

	MapData data;

	// TODO 6: Short function to get the value of x,y
	inline uint Get( int x,  int y) const
	{
		return  (y * data.width + x);
	}

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};



#endif // __j1MAP_H__