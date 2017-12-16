#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include <math.h>
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "Brofiler\Brofiler.h"


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

	return ret;
}

void j1Map::Draw()
{
	BROFILER_CATEGORY("Draw_Map", Profiler::Color::DarkMagenta);
	if(map_loaded == false)
		return;

	p2List_item<TileSet*>* tiles = data.tilesets.start;
	while (tiles != nullptr) {
		p2List_item<map_layer*>* layer = data.layers.start;
		p2List_item<map_layer*>* collisions = data.collisions.start;
		while (layer != nullptr) {
			for (uint i = 0; i < layer->data->width; i++) {
				for (uint j = 0; j < layer->data->height; j++) {

					if (layer->data->Get(i, j) != 0) {
						iPoint pos = MapToWorld(i, j);
						SDL_Rect* square = &tiles->data->GetTileRect(layer->data->Get(i, j));

						if (layer->data->id == PARALLAX)
							App->render->Blit(tiles->data->texture, pos.x, pos.y, square, 0.7);
						else
							App->render->Blit(tiles->data->texture, pos.x, pos.y, square);
					}
				}
			}
			layer = layer->next;
		}
		while (collisions != nullptr) {
			for (uint i = 0; i < collisions->data->width; i++) {
				for (uint j = 0; j < collisions->data->height; j++) {

					if (collisions->data->Get(i, j) != 0) {
						iPoint pos = MapToWorld(i, j);
						SDL_Rect* square = &tiles->data->GetTileRect(collisions->data->Get(i, j));

						if (collisions->data->id == COLLISION && view_collision == true)
							App->render->Blit(tiles->data->texture, pos.x, pos.y, square);
					}
				}
			}
			collisions = collisions->next;
		}
		tiles = tiles->next;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------
inline uint map_layer::Get(int x, int y)const {
	int aux;
	aux = data[y*width + x];
	return aux;
}

//-----------------------------------------------------------------------------------------------------------------------------------

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret;

	if (x < 0)
		ret.x = -1;
	else
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;

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

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	p2List_item<map_layer*>* item0;
	item0 = data.layers.start;

	while (item0 != NULL) {
		RELEASE(item0->data);
		item0 = item0->next;
	}
	data.layers.clear();

	p2List_item<map_layer*>* item1;
	item1 = data.collisions.start;
	while (item1 != NULL) {
		RELEASE(item1->data);
		item1 = item1->next;
	}
	data.collisions.clear();

	data.objects.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	//p2SString tmp("%s%s", folder.GetString(), file_name);

	p2SString tmp(PATH(folder.GetString(), file_name));
	

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}


	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer")) {
		map_layer* layer0 = new map_layer();

		if (ret == true)
			ret = LoadLayer(layer, layer0);

		if (layer0->id == COLLISION)
			data.collisions.add(layer0);
		else
			data.layers.add(layer0);
	}

	pugi::xml_node mapObject;
	for (mapObject = map_file.child("map").child("objectgroup"); mapObject && ret; mapObject = mapObject.next_sibling("objectgroup")) {
		map_object* object0 = new map_object();

		if (ret == true)
			ret = LoadObject(mapObject, object0);

		data.objects.add(object0);
	}


	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}
		
		p2List_item<map_layer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			map_layer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
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

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
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

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = (set->tex_width + set->margin *2 )/ (set->tile_width + set->spacing);
		set->num_tiles_height = (set->tex_height + set->margin * 2)/ (set->tile_height + set->spacing);
	}

	return ret;
}

bool j1Map::LoadObject(pugi::xml_node& node, map_object* object)
{
	bool ret = true;
	if (node == NULL) {
		LOG("Could not load Object");
		ret = false;
	}

	else {
		object->name = node.attribute("name").as_string();
		for (pugi::xml_node object_node = node.child("object"); object_node; object_node = object_node.next_sibling("object")) {
			object->id = object_node.attribute("id").as_uint();
			object->x = object_node.attribute("x").as_uint();
			object->y = object_node.attribute("y").as_uint();
			object->width = object_node.attribute("width").as_uint();
			object->height = object_node.attribute("height").as_uint();
			App->player->startPos.x = object->x;
			App->player->startPos.y = object->y;
		}
	}
	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, map_layer* layer)
{
	// Read all tiled layers to paint it, TO DO Valdivia
	bool ret = true;
	if (node == NULL) {
		LOG("Could not load layer");
		ret = false;
	}

	else {
		layer->name = node.attribute("name").as_string();

		if (layer->name == "Parallax") {
			layer->id = PARALLAX;
		}
		else if (layer->name == "Front") {
			layer->id = FRONT;
		}
		else if (layer->name == "Collision") {
			collisions = layer;
			layer->id = COLLISION;
		}

		layer->name = node.attribute("name").as_string();
		layer->width = node.attribute("width").as_uint();
		layer->height = node.attribute("height").as_uint();
		layer->size_data = layer->width * layer->height;
		layer->data = new uint[layer->size_data];

		memset(layer->data, 0, layer->size_data);

		int i = 0;
		for (pugi::xml_node gid_node = node.child("data").child("tile"); gid_node; gid_node = gid_node.next_sibling("tile")) {
			layer->data[i] = gid_node.attribute("gid").as_uint();
			i++;
		}
	}
	return ret;
}
// Add collisions to the maps, TO DO Varela
bool j1Map::CollisionX(uint x, uint y_up, uint y_down)
{
	if (x < 0 || x > data.width)
		return true;
	p2List_item<map_layer*>* collisions = data.collisions.start;
	for (uint y = y_up; y <= y_down; y++)
	{
		if (collisions->data->Get(x, y) != 0)
			return true;
	}
	return false;
}

bool j1Map::CollisionY(uint x_left, uint x_right, uint y)
{
	if (y < 0) 
		return true;
	if (y >= data.height && App->player->GetLives() > 0) {
		App->fade_to_black->FadeToBlack(this, this, 1);
		App->player->lives--;
		App->audio->PlayFx(2, 0);
		App->scene->MapStart();
		return true;
	}
	p2List_item<map_layer*>* collisions = data.collisions.start;
	for (uint x = x_left; x <= x_right; x++)
	{
		if (collisions->data->Get(x, y) != 0)
			return true;
	}
	return false;
}
	