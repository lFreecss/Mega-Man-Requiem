#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1map.h"
#include "j1Enemies.h"
#include "Enemy.h"
#include "Blader.h"

j1Enemies::j1Enemies() : j1Module()
{
	name.create("enemies");

}
j1Enemies::~j1Enemies() {

}

bool j1Enemies::Awake(pugi::xml_node& config) {
	bool ret = true;

	return ret;
}

bool j1Enemies::Start() {
	bool ret = true;

	sprites = App->tex->Load("textures/enemies.png");

	return ret;
}

bool j1Enemies::Update(float dt) {
	bool ret = true;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Draw(sprites);

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
		case ENEMY_TYPES::AIR:
			enemies[i] = new Blader(info.x, info.y);
			enemies[i]->type = ENEMY_TYPES::AIR;
		}
	}
}