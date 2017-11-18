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
	int id;
};
struct EnemyTypeInfo {
	iPoint size;
	float speed;
	float gravity;
	uint positioning_barrier;
	uint radar_limit_1;
	uint radar_limit_2;
	uint ending_radar_limit;
	float anim1_speed;
	float anim2_speed;
};


class j1Enemies : public j1Module {
public:
	j1Enemies();
	~j1Enemies();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENEMY_TYPES type, int x, int y);
	EnemyTypeInfo& BInfo() { return blader_info; }
	EnemyTypeInfo& CRInfo() { return crazy_razy_info; }
	//Like CleanUp, deletes all enemies, but doesn't clear up the spritesheet
	void DeleteEnemy();

	Enemy* enemies[MAX_ENEMIES];

private:

	void SpawnEnemy(const EnemyInfo&);

	SDL_Texture* sprites;
	p2SString path;
	EnemyInfo queue[MAX_ENEMIES];

	int screen_width;
	int screen_height;
	int screen_size;

	EnemyTypeInfo blader_info;
	EnemyTypeInfo crazy_razy_info;
};

#endif