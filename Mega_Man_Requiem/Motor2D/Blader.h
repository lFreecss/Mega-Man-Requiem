#ifndef __BALDER_H__
#define __BLADER_H__

#include "Enemy.h"

class Blader : public Enemy
{
private:
	Animation idle;
	iPoint original_pos;

public:
	Blader(int x, int y);
	void Move();
	void OnCollision(Collider* collider);
};


#endif