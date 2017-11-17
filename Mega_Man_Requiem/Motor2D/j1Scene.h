#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

enum LEVEL_ID {
	NO_LEVEL = 0,
	ROCK,
	JAIL
};

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

	//Checks if it is the correct map in case of applying a save state
	void CheckMap();

	//Change map
	void ChangeMaps(LEVEL_ID level_name);

	//Start from 1st level
	void Restart();

	//Start from actual level
	void MapStart();

	//Restart map id
	void InitializeMap();

	//Initialize Enemies
	void EnemySpawn();

	//Change enemies from level to level
	void EnemyInitialation();

	//Function for the debug keys
	void DebugKeys();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	p2SString first_map;
	const char* current_map;
	int map_num;
	p2SString music_path;
	float scroll_speed;

	p2SString rock_level;
	p2SString jail_level;
	LEVEL_ID current_level = NO_LEVEL;

	iPoint air_enem_1_1;
	iPoint ground_enem_1_1;
	iPoint air_enem_1_2;
	iPoint ground_enem_1_2;
	iPoint air_enem_2_1;
	iPoint ground_enem_2_1;
	iPoint air_enem_2_2;
	iPoint ground_enem_2_2;
};

#endif // __j1SCENE_H__