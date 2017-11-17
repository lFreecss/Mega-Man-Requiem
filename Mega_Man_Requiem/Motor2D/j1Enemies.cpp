#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1map.h"
#include "j1Enemies.h"
#include "Enemy.h"
#include "Blader.h"
#include "Crazy_Razy.h"

j1Enemies::j1Enemies() : j1Module()
{
	name.create("enemies");
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		enemies[i] = nullptr;
	}

}
j1Enemies::~j1Enemies() {

}

bool j1Enemies::Awake(pugi::xml_node& config) {
	bool ret = true;
	//path = config.child("file").attribute("name").as_string();
	path.create(config.child("file").attribute("name").as_string());
	for (uint i = 0; i < MAX_ENEMIES; ++i) {
		if (enemies[i] != nullptr)
			enemies[i]->Awake(config);
	}
	return ret;
}

bool j1Enemies::Start() {
	bool ret = true;

	sprites = App->tex->Load(path.GetString());
	uint sheight;
	uint swidth;
	App->win->GetWindowSize(swidth, sheight);
	screen_size = (int)App->win->GetScale();
	screen_width = (int)swidth;
	screen_height = (int)sheight;

	return ret;
}

bool j1Enemies::PreUpdate() {
	bool ret = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE)
		{
			if (queue[i].y >(abs(App->render->camera.y) / screen_size)) //-SPAWN_MARGIN
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENEMY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y);
			}
		}
	}
	return ret;
}

bool j1Enemies::Update(float dt) {
	bool ret = true;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr)
			enemies[i]->Move(dt);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) 
			enemies[i]->Draw(sprites);

	return ret;
}

bool j1Enemies::PostUpdate() {
	bool ret = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			if (enemies[i]->pos.y >((abs(App->render->camera.y) + screen_height) / screen_width) + 240)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->pos.y);
				delete enemies[i];
				enemies[i] = nullptr;
			}
		}
	}
	return ret;
}

bool j1Enemies::CleanUp() {
	bool ret = true;

	LOG("Freeing all enemies");

	App->tex->UnLoad(sprites);

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return ret;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			queue[i].id = i;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info) {
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::GROUND:
			enemies[i] = new Crazy_Razy(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::GROUND;
			enemies[i]->id = info.id;
			break;
		case ENEMY_TYPES::AIR:
			enemies[i] = new Blader(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::AIR;
			enemies[i]->id = info.id;
			break;
		}
	}
}

void j1Enemies::DeleteEnemy() {

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
	}
}