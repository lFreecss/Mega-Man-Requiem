#include "j1App.h"
#include "Blader.h"

Blader::Blader(int x, int y) : Enemy(x, y)
{
	original_pos.y = y;
	original_pos.x = x;
}

void Blader::Move() {



}

void OnCollision(Collider* collider) {



}