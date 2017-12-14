#ifndef __j1ENEMIES_H__
#define __j1ENEMIES_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"
#include "Entity.h"

#define MAX_ENTITIES 100
class Letter;

enum ENTITY_TYPES
{
	NO_TYPE,
	GROUND,
	LETTER,
	AIR
};

struct EntityInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
	int id;
};
struct EntityTypeInfo {
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


class j1Entities : public j1Module {
public:
	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);
	bool AddEnemy(ENTITY_TYPES type, int x, int y);
	EntityTypeInfo& BInfo() { return blader_info; }
	EntityTypeInfo& CRInfo() { return crazy_razy_info; }
	//Like CleanUp, deletes all enemies, but doesn't clear up the spritesheet
	void DeleteEnemy();
	void DeleteLetters();

	Entity* entities[MAX_ENTITIES];
	
private:

	void SpawnEnemy(const EntityInfo&);

	SDL_Texture* sprites;
	p2SString path;
	EntityInfo queue[MAX_ENTITIES];

	int screen_width = 0;
	int screen_height = 0;
	int screen_size = 0;

	EntityTypeInfo blader_info;
	EntityTypeInfo crazy_razy_info;
};

#endif