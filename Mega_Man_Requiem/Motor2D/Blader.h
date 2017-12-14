#ifndef __BLADER_H__
#define __BLADER_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Entities.h"

class Blader : public Entity
{
private:

	Animation idle;
	iPoint original_pos = { 0,0 };
	iPoint player_pos = { 0,0 };
	int iteration = 0;
	int destination = 0;
	EntityTypeInfo b;

public:

	Blader(int x, int y);
	void Move(float dt);
	void UpdateAnim(float dt);
	void OnCollision(Collider* collider);
	void CreatePath();
	void FollowPath(float dt);
	
};


#endif