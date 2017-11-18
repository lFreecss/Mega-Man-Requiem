#ifndef __BALDER_H__
#define __BLADER_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "j1Enemies.h"

class Blader : public Enemy
{
private:

	Animation idle;
	iPoint original_pos;
	iPoint player_pos;
	int iteration;
	int destination;
	EnemyTypeInfo b;

public:

	Blader(int x, int y);
	void Move(float dt);
	void OnCollision(Collider* collider);
	void CreatePath();
	void FollowPath(float dt);
	
};


#endif