#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Animation.h"

struct SDL_Texture;

class j1Player : public j1Module  {

public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node&);
	bool Start();
	void Init();
	bool Update(float dt);
	bool CleanUp();

	void jump(float dt);
	void move(float dt);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	fPoint startPos;
	fPoint pos;

	void OnCollision(Collider* c1, Collider* c2);
	Collider* collider = nullptr;

	bool GodMode();
	bool is_invincible = false;
private:
	
	Animation* current_animation;
	Animation idle;
	Animation right;
	Animation left;
	Animation jumpR;
	Animation sJump;
	Animation idle_inv;
	Animation right_inv;
	Animation left_inv;
	Animation jumpR_inv;
	Animation sJump_inv;

	SDL_Texture* graphics = nullptr;
	p2SString path;

	fPoint vel;
	float gravity;

	int jumping;
	uint jumpframes;
	uint actualJumpframes;
	iPoint size;
	uint floor_level;
};

#endif