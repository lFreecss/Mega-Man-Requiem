#ifndef __CRAZY_RAZY_H__
#define __CRAZY_RAZY_H__

#include "Enemy.h"

class Crazy_Razy : public Enemy
{
private:
	Animation left;
	Animation right;
	iPoint original_pos;

public:
	Crazy_Razy(int x, int y);
	void Move();
	void OnCollision(Collider* collider);
};


#endif