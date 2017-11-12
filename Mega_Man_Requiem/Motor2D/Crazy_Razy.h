#ifndef __CRAZY_RAZY_H__
#define __CRAZY_RAZY_H__

#include "Enemy.h"
#include "Path.h"
#include "p2DynArray.h"
#include "p2Point.h"

class Crazy_Razy : public Enemy
{
private:
	Animation left;
	Animation right;
	iPoint original_pos;
	iPoint player_pos;
	iPoint size;
	Path walk;
	const p2DynArray<iPoint>* path;

public:
	Crazy_Razy(int x, int y);
	void Move(float dt);
	void OnCollision(Collider* collider);

	int iteration;
	int destination;
	void CreatePath();
	void FollowPath();

};


#endif