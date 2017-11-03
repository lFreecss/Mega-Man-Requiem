#ifndef __ENEMY_BALLOON_H__
#define __ENEMY_BALLOON_H__

#include "Enemy.h"

class Blader : public Enemy
{
private:
	Animation idle;
	iPoint original_pos;

public:
	Blader(int x, int y);
	void Move();
};


#endif