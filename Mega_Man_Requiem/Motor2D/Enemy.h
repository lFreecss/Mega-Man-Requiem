#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class Enemy
{
protected:
	Animation* animation = nullptr;

public:

	iPoint pos;
	Collider* collider = nullptr;
	int type;
	int id;

public:
	Enemy(int x, int y);
	virtual ~Enemy();

	const Collider* GetCollider() const;
	virtual void Awake(pugi::xml_node& config);
	virtual void Move(float dt);
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	virtual void CreatePath();
	virtual void FollowPath(float dt);
};


#endif