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

	void updateAnim(float dt);

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	fPoint startPos = { 0.0f, 0.0f };
	fPoint pos = { 0.0f,0.0f };

	void OnCollision(Collider* c1, Collider* c2);
	Collider* collider = nullptr;

	bool GodMode();
	bool is_invincible = false;
	uint GetLives();
	Collider* GetCollider();

	uint lives = 3;
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
	float idle_speed = 0.0f;
	float right_speed = 0.0f;
	float left_speed = 0.0f;
	float jumpR_speed = 0.0f;
	float sJump_speed = 0.0f;
	float idle_inv_speed = 0.0f;
	float right_inv_speed = 0.0f;
	float left_inv_speed = 0.0f;
	float jumpR_inv_speed = 0.0f;
	float sJump_inv_speed = 0.0f;

	SDL_Texture* graphics = nullptr;
	p2SString path;
	
	fPoint vel = { 0.0f, 0.0f };
	float gravity = 0.0f;

	int jumping = 0;
	float jumpMaxTime = 0.0f;
	float actualJumpTime = 0.0f;
	iPoint size = { 0,0 };
	uint floor_level = 0;

	bool landed = false;
};

#endif