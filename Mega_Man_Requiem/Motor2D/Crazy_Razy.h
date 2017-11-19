#ifndef __CRAZY_RAZY_H__
#define __CRAZY_RAZY_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Enemies.h"

class Crazy_Razy : public Enemy
{
private:

	Animation left;
	Animation right;
	iPoint original_pos;
	iPoint player_pos;
	int iteration;
	int destination;
	EnemyTypeInfo cr;

public:

	Crazy_Razy(int x, int y);
	void Move(float dt);
	void UpdateAnim(float dt);
	void OnCollision(Collider* collider);
	void CreatePath();
	void FollowPath(float dt);

};


#endif