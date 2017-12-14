#ifndef __CRAZY_RAZY_H__
#define __CRAZY_RAZY_H__

#include "Entity.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Entities.h"

class Crazy_Razy : public Entity
{
private:

	Animation left;
	Animation right;
	iPoint original_pos = { 0,0 };
	iPoint player_pos = { 0,0 };
	int iteration = 0;
	int destination = 0;
	EntityTypeInfo cr;

public:

	Crazy_Razy(int x, int y);
	void Move(float dt);
	void UpdateAnim(float dt);
	void OnCollision(Collider* collider);
	void CreatePath();
	void FollowPath(float dt);

};


#endif