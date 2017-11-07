#include "j1App.h"
#include "Blader.h"
#include "j1Collision.h"

Blader::Blader(int x, int y) : Enemy(x, y)
{
	original_pos.y = y;
	original_pos.x = x;
	//collider = App->collision->AddCollider({ 0, 0, 43, 53 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);
}

void Blader::Move() {



}

void Blader::OnCollision(Collider* collider) {



}