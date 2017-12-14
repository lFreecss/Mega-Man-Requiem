#ifndef __LETTER_H__
#define __LETTER_H__

#include "Entity.h"
#include "p2Point.h"

class Letter : public Entity {

public:
	Letter(int x, int y);
	void UpdateAnim(float dt);
private:
	Animation idle;
	iPoint size = { 0,0 };
};


#endif