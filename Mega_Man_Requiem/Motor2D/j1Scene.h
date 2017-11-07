#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Change map
	void ChangeMaps(const char*);

	//Start from 1st level
	void Restart();

	//Start from actual level
	void MapStart();

	//Restart map id
	void InitializeMap();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	p2SString first_map;
	const char* current_map;
	int map_num;
	const char* music_path;
	float scroll_speed;

	const char* rock_level;
	const char* jail_level;
	uint id[2];
};

#endif // __j1SCENE_H__