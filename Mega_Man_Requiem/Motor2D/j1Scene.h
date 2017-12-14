#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
class Image;
class Label;
class Button;

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

	//Goes to the Start
	void StartScreen();

	//Starts the playable game part
	void StartPlaying();

	//Goes to the Settings screen
	void SettingsScreen();

	//Goes to the Credits screen
	void CreditsScreen();

	//Goes to the Game Over screen
	void GameOverScreen();

	//Goes to the end screen
	void EndScreen();

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

	//Initialize the letters item
	void LetterInitialation();

	//Function for the debug keys
	void DebugKeys();

	//UI interaction
	void UIInteraction(UI* UI_elem, BUTTON_EVENTS UI_state);

	//Manages the in game UI
	void ManageStageUI();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	int punctuation_count = 0;

private:
	p2SString first_map;
	const char* current_map;
	int map_num = 0;
	float scroll_speed = 0.0f;
	uint scroll_limit = 0;
	uint map_limit = 0;

	p2SString music_title;
	p2SString music_settings;
	p2SString music_1st_lvl;
	p2SString music_2nd_lvl;
	p2SString sound_landing;
	p2SString sound_defeat;
	p2SString sound_start;
	p2SString sound_button_select;

	p2SString rock_level;
	p2SString jail_level;
	LEVEL_ID current_level = NO_LEVEL;

	iPoint air_enem_1_1 = { 0,0 };
	iPoint ground_enem_1_1 = { 0,0 };
	iPoint air_enem_1_2 = { 0,0 };
	iPoint ground_enem_1_2 = { 0,0 };
	iPoint air_enem_2_1 = { 0,0 };
	iPoint ground_enem_2_1 = { 0,0 };
	iPoint air_enem_2_2 = { 0,0 };
	iPoint ground_enem_2_2 = { 0,0 };

	SDL_Texture* title_bg;
	SDL_Texture* buttons;
	SDL_Texture* settings_bg;
	SDL_Texture* settings_scrn;
	SDL_Texture* items;
	p2SString title_bg_path;
	p2SString buttons_path;
	p2SString settings_bg_path;
	p2SString settings_scrn_path;
	p2SString items_path;

	Button* start_bttn = nullptr;
	Button* load_bttn = nullptr;
	Button* settings_bttn = nullptr;
	Button* credits_bttn = nullptr;
	Button* quit_bttn = nullptr;
	Button* back_bttn = nullptr;
	Button* music_off = nullptr;
	Button* plus_volume = nullptr;
	Button* minus_volume = nullptr;

	Label* life_count = nullptr;
	Label* punctuation = nullptr;

	Image* letter_M_1 = nullptr;
	Image* letter_E = nullptr;
	Image* letter_G = nullptr;
	Image* letter_A_1 = nullptr;
	Image* letter_M_2 = nullptr;
	Image* letter_A_2 = nullptr;
	Image* letter_N = nullptr;
	Image* letter_R = nullptr;

	bool quit_pressed = false;

	char volume = 128;
	bool music_off_vol = true;
};

#endif // __j1SCENE_H__