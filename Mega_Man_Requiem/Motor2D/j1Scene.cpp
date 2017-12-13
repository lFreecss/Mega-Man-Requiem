#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Collision.h"
#include "j1Enemies.h"
#include "j1Gui.h"
#include "j1Textures.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	bool ret = true;
	LOG("Loading Scene");

	scroll_speed = config.child("scroll").attribute("speed").as_float();
	scroll_limit = config.child("scroll").attribute("limit").as_uint();
	map_limit = config.child("map").attribute("limit").as_uint();
	rock_level.create(config.child("levels").child("first_level").attribute("name").as_string());
	jail_level.create(config.child("levels").child("second_level").attribute("name").as_string());
	
	LOG("Charging all audio");
	music_title.create(config.child("music").child("menus").attribute("title").as_string());
	music_settings.create(config.child("music").child("menus").attribute("setttings").as_string());
	music_1st_lvl.create(config.child("music").child("level").attribute("guts_man").as_string());
	music_2nd_lvl.create(config.child("music").child("level").attribute("cut_man").as_string());

	LOG("Charging all enemy positions");
	pugi::xml_node enemy_pos = config.child("enemy_pos");
	air_enem_1_1.x = enemy_pos.child("Air1-1").attribute("x").as_int();
	air_enem_1_1.y = enemy_pos.child("Air1-1").attribute("y").as_int();
	ground_enem_1_1.x = enemy_pos.child("Ground1-1").attribute("x").as_int();
	ground_enem_1_1.y = enemy_pos.child("Ground1-1").attribute("y").as_int();
	air_enem_1_2.x = enemy_pos.child("Air1-2").attribute("x").as_int();
	air_enem_1_2.y = enemy_pos.child("Air1-2").attribute("y").as_int();
	ground_enem_1_2.x = enemy_pos.child("Ground1-2").attribute("x").as_int();
	ground_enem_1_2.y = enemy_pos.child("Ground1-2").attribute("y").as_int();
	air_enem_2_1.x = enemy_pos.child("Air2-1").attribute("x").as_int();
	air_enem_2_1.y = enemy_pos.child("Air2-1").attribute("y").as_int();
	ground_enem_2_1.x = enemy_pos.child("Ground2-1").attribute("x").as_int();
	ground_enem_2_1.y = enemy_pos.child("Ground2-1").attribute("y").as_int();
	air_enem_2_2.x = enemy_pos.child("Air2-2").attribute("x").as_int();
	air_enem_2_2.y = enemy_pos.child("Air2-2").attribute("y").as_int();
	ground_enem_2_2.x = enemy_pos.child("Ground2-2").attribute("x").as_int();
	ground_enem_2_2.y = enemy_pos.child("Ground2-2").attribute("y").as_int();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->player->active = false;
	App->map->active = false;
	App->enemies->active = false;
	App->collision->active = false;

	App->audio->LoadFx("audio/fx/mega_man_landing.wav"); //1
	App->audio->LoadFx("audio/fx/mega_man_defeat.wav"); //2
	App->audio->LoadFx("audio/fx/game_start.wav"); //3
	App->audio->LoadFx("audio/fx/menu_select.wav"); //4

	title_bg = App->tex->Load("textures/title.png");
	buttons = App->tex->Load("textures/buttons.png");
	settings_bg = App->tex->Load("textures/settings_bg.png");
	settings_scrn = App->tex->Load("textures/settings_screen.png");
	items = App->tex->Load("textures/items.png");

	StartScreen();
	//Mix_VolumeChunk(chunk,0);
	return true;
}


void j1Scene::UIInteraction(UI* UI_elem, BUTTON_EVENTS UI_state)
{
	if (UI_elem->GetType() == BUTTON && UI_elem != music_off) {
		Button* bttn = (Button*)UI_elem;
		switch (UI_state)
		{
		case NONE:
			break;
		case MOUSE_ENTER:
			if(bttn != music_off && bttn != plus_volume && bttn != minus_volume)
			App->audio->PlayFx(4, 0);
			break;
		case MOUSE_INSIDE:
			bttn->ChangeToHoverImg();
			break;
		case MOUSE_LEAVE:
			bttn->ChangeToNormalImg();
			break;
		case LEFT_MOUSE_PRESS:
			bttn->ChangeToPressedImg();

			if (bttn == start_bttn) {
				App->audio->PlayFx(3, 0);
				StartPlaying();
			}

			else if (bttn == load_bttn) {
				App->audio->PlayFx(3, 0);
				StartPlaying();
				App->LoadGame();
			}

			else if (bttn == settings_bttn)
				SettingsScreen();

			else if (bttn == credits_bttn)
				CreditsScreen();

			else if (bttn == plus_volume) {
				music_off_vol = true;
				volume++;
				Mix_VolumeMusic(volume);
			}

			else if (bttn == minus_volume) {
				volume--;
				if (volume < 0) {
					volume = 0;
					music_off_vol = false;
				}
				Mix_VolumeMusic(volume);
			}

			else if (bttn == back_bttn)
				StartScreen();
			
			else if (bttn == quit_bttn)
					quit_pressed = true;

			break;
		case RIGHT_MOUSE_PRESS:
			bttn->ChangeToPressedImg();
			break;
		}
	}

	//Checkbox for turning on/off the music
	if (UI_elem == music_off) {
		Button* bttn = (Button*)UI_elem;
		switch (UI_state) {
		case NONE:
			if (music_off_vol) {
				bttn->ChangeToNormalImg();
			}
			else if (music_off_vol == false) {
				bttn->ChangeToPressedImg();
			}
			break;
		case LEFT_MOUSE_PRESS:
			if (music_off_vol) {
				music_off_vol = false;
				volume = 0;
				Mix_VolumeMusic(0);
				bttn->ChangeToPressedImg();
			}
			else if (music_off_vol == false) {
				music_off_vol = true;
				volume = 128;
				Mix_VolumeMusic(128);
				bttn->ChangeToNormalImg();
			}
			break;
		}
	}
}


// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->map->active) {
		CheckMap();
		App->map->Draw();
		DebugKeys();
	}

	//Scroll
	if (App->player->pos.x <= App->map->data.tile_width*App->map->data.width - scroll_limit)
		App->render->camera.x = -(scroll_speed * App->player->pos.x);

	
	//When player gets at the end of the level, change to the next
	if (App->player->pos.x >= App->map->data.tile_width*App->map->data.width - map_limit) {
		App->collision->EraseCollider(App->player->collider);

		if (current_level == ROCK) {
			ChangeMaps(JAIL);
			MapStart();
		}

		else {
			ChangeMaps(ROCK);
			MapStart();
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::StartScreen() {
	music_off = nullptr;
	plus_volume = nullptr;
	minus_volume = nullptr;
	back_bttn = nullptr;

	App->gui->CleanUp();
	App->audio->PlayMusic("audio/music/title.ogg", 0.0f);

	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, title_bg, false, this); //Title image
	start_bttn = App->gui->CreateButton({ 60,170 }, { 6, 10, 38, 7 }, { 49, 10, 38, 8 }, { 6, 10, 38, 7 }, buttons, false, this);
	load_bttn = App->gui->CreateButton({ 60,190 }, { 9, 28, 30, 7 }, { 50, 27, 30, 8 }, { 9, 28, 30, 7 }, buttons, false, this);
	settings_bttn = App->gui->CreateButton({ 60,210 }, { 7, 43, 60, 7 }, { 76, 43, 60, 8 }, { 7, 43, 60, 7 }, buttons, false, this);
	credits_bttn = App->gui->CreateButton({ 60,230 }, { 6, 84, 52, 7 }, { 65, 84, 52, 8 }, { 6, 84, 52, 7 }, buttons, false, this);
	quit_bttn = App->gui->CreateButton({ 60,250 }, { 7, 57, 28, 7 }, { 42, 57, 28, 8 }, { 7, 57, 28, 7 }, buttons, false, this);
}

void j1Scene::StartPlaying() {
	App->gui->CleanUp();
	App->player->active = true;
	App->map->active = true;
	App->enemies->active = true;
	App->collision->active = true;

	first_map = rock_level;
	current_map = rock_level.GetString();
	App->map->Load(first_map.GetString());
	InitializeMap();
	App->audio->PlayMusic("audio/music/guts_man.ogg", 0.0f);

	App->gui->CreateImage({ 5,255 }, { 4, 85, 19, 18 }, items, false, this); //Lives
	App->gui->CreateLabel({ 30,260 }, "X3", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); //Live count
	App->gui->CreateLabel({ 170,10 }, "0000000", App->gui->GetFont(MEGA_MAN_10_SIZE_12), { 255,255,255,255 }, false, this); //Punctuation
	
	App->gui->CreateImage({ 130,30 }, { 5, 31, 16, 16 }, items, false, this); //M
	App->gui->CreateImage({ 150,30 }, { 24, 31, 16, 16 }, items, false, this); //E
	App->gui->CreateImage({ 170,30 }, { 43, 31, 16, 16 }, items, false, this); //G
	App->gui->CreateImage({ 190,30 }, { 62, 31, 16, 16 }, items, false, this); //A
	App->gui->CreateImage({ 210,30 }, { 5, 31, 16, 16 }, items, false, this); //M 2
	App->gui->CreateImage({ 230,30 }, { 62, 31, 16, 16 }, items, false, this); //A 2
	App->gui->CreateImage({ 250,30 }, { 81, 31, 16, 16 }, items, false, this); //N
	App->gui->CreateImage({ 270,30 }, { 100, 31, 16, 16 }, items, false, this); //R
	EnemySpawn();
}

void j1Scene::CreditsScreen() {
	App->gui->CleanUp();
	start_bttn = nullptr;
	load_bttn = nullptr;
	settings_bttn = nullptr;
	credits_bttn = nullptr;
	quit_bttn = nullptr;

	
	App->audio->PlayMusic("audio/music/password.ogg", 0.0f);
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateImage({ 30,40 }, { 0, 0, 368, 206 }, settings_scrn, false, this); //Settings/Credits black screen
	
	App->gui->CreateLabel({ 50, 20 }, "Licence:", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 255,255,255,255 }, false, this);
	App->gui->CreateLabel({ 40, 50 }, "MIT License Copyright(c)", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	App->gui->CreateLabel({ 40, 60 }, "David Varela Hernandez, David Valdivia Martínez", App->gui->GetFont(MEGA_MAN_10_SIZE_8), { 0,255,0,255 }, false, this);
	back_bttn = App->gui->CreateButton({ 10,250 }, { 7, 71, 31, 7 }, { 43, 71, 31, 8 }, { 7, 71, 31, 7 }, buttons, false, this);
}

void j1Scene::SettingsScreen() {
	App->gui->CleanUp();
	start_bttn = nullptr;
	load_bttn = nullptr;
	settings_bttn = nullptr;
	credits_bttn = nullptr;
	quit_bttn = nullptr;

	
	App->audio->PlayMusic("audio/music/password.ogg", 0.0f);
	
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG
	App->gui->CreateImage({ 110,60 }, { 0, 0, 184, 123 }, settings_scrn, false, this); //Settings/Credits black screen
	
	App->gui->CreateLabel({ 170, 20 }, "SETTINGS", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	App->gui->CreateLabel({ 120, 80 }, "MUSIC OFF", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	music_off = App->gui->CreateButton({ 250,70 }, { 160, 26, 22, 24 }, { 160, 26, 22, 24 }, { 188, 26, 24, 24 }, buttons, false, this); //Checkbox
	
	App->gui->CreateLabel({ 120, 130 }, "MUSIC VOLUME", App->gui->GetFont(MEGA_MAN_2), { 255,255,255,255 }, false, this);
	plus_volume = App->gui->CreateButton({ 150,150 }, { 164, 59, 16, 16 }, { 164, 59, 16, 16 }, { 205, 59, 16, 16 }, buttons, false, this); //Plus button
	minus_volume = App->gui->CreateButton({ 230,150 }, { 183, 59, 16, 16 }, { 183, 59, 16, 16 }, { 224, 59, 16, 16 }, buttons, false, this); //Minus button

	back_bttn = App->gui->CreateButton({ 10,250 }, { 7, 71, 31, 7 }, { 43, 71, 31, 8 }, { 7, 71, 31, 7 }, buttons, false, this);
}

void j1Scene::GameOverScreen() {
	App->gui->CleanUp();
	App->player->active = false;
	App->map->active = false;
	App->enemies->active = false;
	App->collision->active = false;
	App->player->CleanUp();
	App->enemies->CleanUp();
	App->collision->CleanUp();
	//App->map->CleanUp();
	//App->audio->CleanUp();
	App->gui->CreateImage({ 0,0 }, { 0, 0, 427, 287 }, settings_bg, false, this); //Settings/Credits BG

}

void j1Scene::DebugKeys(){

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Restart();

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		MapStart();

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN){
		App->collision->view_collision = !App->collision->view_collision;
		App->map->view_collision = !App->map->view_collision;
	}
		
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		App->player->is_invincible = !App->player->is_invincible;

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		App->framerate_capped = !App->framerate_capped;
	}
	    
	//Change the level
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		App->player->pos.x = App->map->data.tile_width*App->map->data.width - 30;
	}

}

void j1Scene::CheckMap() {

	if (map_num == 0 && current_level != ROCK) {
		ChangeMaps(ROCK);
		EnemyInitialation();
	}
	if (map_num == 1 && current_level != JAIL) {
		ChangeMaps(JAIL);
		EnemyInitialation();
	}

}

//Change from one map to the other, TODO Valdivia
void j1Scene::ChangeMaps(LEVEL_ID level_name) {
	App->map->CleanUp();
	
	if (level_name == ROCK) {
		App->map->Load(rock_level.GetString());
		current_map = rock_level.GetString();
		App->audio->PlayMusic("audio/music/guts_man.ogg", 0.0f);
	}
	if (level_name == JAIL) {
		App->map->Load(jail_level.GetString());
		current_map = jail_level.GetString();
		App->audio->PlayMusic("audio/music/cut_man.ogg", 0.0f);
	}
	InitializeMap();
}

//For starting form the same map, TODO Varela
void j1Scene::InitializeMap() {
	if (current_map == rock_level.GetString()) {
		current_level = ROCK;
		map_num = 0;
	}
	if (current_map == jail_level.GetString()) {
		current_level = JAIL;
		map_num = 1;
	}
}

//To Start form the very first level, TODO Varela
void j1Scene::Restart() {
	ChangeMaps(ROCK);
	MapStart();
	map_num = 0;
}

void j1Scene::MapStart() {
	App->render->camera.x = 0;
	App->render->camera.x = 0;
	App->player->Init();
	EnemyInitialation();
}

//Initialize enemies for each level
void j1Scene::EnemyInitialation() {

	if (current_level == ROCK) {
		EnemySpawn();
	}
	if (current_level == JAIL) {
		App->enemies->DeleteEnemy();
		App->enemies->AddEnemy(AIR, air_enem_2_1.x, air_enem_2_1.y);
		App->enemies->AddEnemy(GROUND, ground_enem_2_1.x, ground_enem_2_1.y);
		App->enemies->AddEnemy(AIR, air_enem_2_2.x, air_enem_2_2.y);
		App->enemies->AddEnemy(GROUND, ground_enem_2_2.x, ground_enem_2_2.y);
	}

}

//Enemies for the first level
void j1Scene::EnemySpawn() {
	App->enemies->DeleteEnemy();
	App->enemies->AddEnemy(AIR, air_enem_1_1.x, air_enem_1_1.y);
	App->enemies->AddEnemy(GROUND, ground_enem_1_1.x, ground_enem_1_1.y);
	App->enemies->AddEnemy(AIR, air_enem_1_2.x, air_enem_1_2.y);
	App->enemies->AddEnemy(GROUND, ground_enem_1_2.x, ground_enem_1_2.y);
}

//Load map
bool j1Scene::Load(pugi::xml_node& data)
{
	map_num = data.child("map").attribute("name").as_int();
	return true;
}

// Save map
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node map = data.append_child("map");

	map.append_attribute("name") = map_num;

	return true;
}