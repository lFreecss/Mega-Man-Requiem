#ifndef __j1ENEMIES_H__
#define __j1ENEMIES_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"
#include "Enemy.h"

#define MAX_ENEMIES 100

enum ENEMY_TYPES
{
	NO_TYPE,
	GROUND,
	AIR
};

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
};

class j1Enemies : public j1Module {
public:
	j1Enemies();
	~j1Enemies();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

	Enemy* enemies[MAX_ENEMIES];
private:

	void SpawnEnemy(const EnemyInfo&);

private:

	SDL_Texture* sprites;
	const char* path;
	EnemyInfo queue[MAX_ENEMIES];
};

#endif