#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"

struct SDL_Texture;

class Entity
{
protected:
	Animation* animation = nullptr;
	const p2DynArray<iPoint>* path;

public:

	iPoint pos = { 0,0 };
	Collider* collider = nullptr;
	int type;
	int id;

public:
	Entity(int x, int y);
	virtual ~Entity();

	Collider* GetCollider() const;
	virtual void Awake(pugi::xml_node& config);
	virtual void UpdateAnim(float dt);
	virtual void Move(float dt);
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
	virtual void CreatePath();
	virtual void FollowPath(float dt);
};


#endif