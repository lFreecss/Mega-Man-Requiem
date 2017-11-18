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

	music_path.create(config.child("file").attribute("music_name").as_string());
	//music_path = config.child("file").attribute("music_name").as_string();
	scroll_speed = config.child("scroll").attribute("speed").as_float();
	
	/*for (pugi::xml_node level = config.child("levels").child("level"); level; level = config.next_sibling("level")) {
		if (level.attribute("id").as_uint() == 1)
			rock_level = level.attribute("name").as_string();
		if (level.attribute("id").as_uint() == 2)
			jail_level = level.attribute("name").as_string();
	}*/
	//rock_level = config.child("levels").child("first_level").attribute("name").as_string();
	rock_level.create(config.child("levels").child("first_level").attribute("name").as_string());
	//jail_level = config.child("levels").child("second_level").attribute("name").as_string();
	jail_level.create(config.child("levels").child("second_level").attribute("name").as_string());

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
	//first_map = ("%s%s", App->map_name.GetString(), ".tmx");
	first_map = rock_level;
	App->map->Load(first_map.GetString());
	current_map = first_map.GetString();
	InitializeMap();
	App->audio->PlayMusic(music_path.GetString());

	EnemySpawn();
	
	return true;
}


// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	CheckMap();
	DebugKeys();
	App->map->Draw();
/*
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());*/

	//Scroll
	if (App->player->pos.x <= App->map->data.tile_width*App->map->data.width - 64)
		App->render->camera.x = -(scroll_speed * App->player->pos.x);

	
	//When player gets at the end of the level, change to the next
	if (App->player->pos.x >= App->map->data.tile_width*App->map->data.width - 30) {
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

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
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

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		if (App->player->is_invincible == false)
			App->player->is_invincible = true;
		else
			App->player->is_invincible = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT)
		App->render->camera.y -= 1;

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT)
		App->render->camera.y += 1;

	if (App->input->GetKey(SDL_SCANCODE_H) == KEY_REPEAT)
		App->render->camera.x -= 1;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT)
		App->render->camera.x += 1;

	//Debug for the scroll
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		App->player->pos.x = App->map->data.tile_width*App->map->data.width - 130;

	//Change level, for debuging 
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {

		if (current_level == ROCK) 
			ChangeMaps(JAIL);

		else 
			ChangeMaps(ROCK);

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
	}
	else {
		App->map->Load(jail_level.GetString());
		current_map = jail_level.GetString();
	}
	InitializeMap();
	//MapStart();
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
	//first_map.GetString()
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