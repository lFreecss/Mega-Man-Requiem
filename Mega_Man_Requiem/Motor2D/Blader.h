#ifndef __BALDER_H__
#define __BLADER_H__

#include "Enemy.h"
#include "p2DynArray.h"
#include "p2Point.h"

class Blader : public Enemy
{
private:
	Animation idle;
	iPoint original_pos;
	int i;

	const p2DynArray<iPoint>* path;

public:
	Blader(int x, int y);
	void Move();
	void OnCollision(Collider* collider);
};


#endif