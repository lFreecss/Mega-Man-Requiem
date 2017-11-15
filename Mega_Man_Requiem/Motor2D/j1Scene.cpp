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

	music_path = config.child("file").attribute("music_name").as_string();
	scroll_speed = config.child("scroll").attribute("speed").as_float();
	
	for (pugi::xml_node level = config.child("levels").child("level"); level; level = config.next_sibling("level")) {
		if (level.attribute("id").as_uint() == 1)
			rock_level = level.attribute("name").as_string();
		if (level.attribute("id").as_uint() == 2)
			jail_level = level.attribute("name").as_string();
		//if(level.attribute("loaded").as_bool() == true)
		//first_map = level.attribute("name").as_string();
	}
	rock_level = config.child("level").attribute("rock").value();
	jail_level = config.child("level").attribute("jail").value();

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	first_map = ("%s%s", App->map_name.GetString(), ".tmx");
	App->map->Load(first_map.GetString());
	current_map = first_map.GetString();
	InitializeMap();
	App->audio->PlayMusic(music_path);

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
		App->map->Load("rock_level.tmx");
		current_map = "rock_level.tmx";
	}
	else {
		App->map->Load("JAIL.tmx");
		current_map = "JAIL.tmx";
	}
	InitializeMap();
	//MapStart();
}

//For starting form the same map, TODO Varela
void j1Scene::InitializeMap() {
	if (current_map == "rock_level.tmx") {
		current_level = ROCK;
		map_num = 0;
	}
	if (current_map == "JAIL.tmx") {
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
		App->enemies->AddEnemy(AIR, 320, 170);
		App->enemies->AddEnemy(GROUND, 1050, 200);
	}

}

//Enemies for the first level
void j1Scene::EnemySpawn() {
	App->enemies->DeleteEnemy();
	//App->enemies->AddEnemy(GROUND, 70, 170);
	App->enemies->AddEnemy(AIR, 260, 170);
	App->enemies->AddEnemy(GROUND, 550, 170);
	App->enemies->AddEnemy(AIR, 1200, 200);
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