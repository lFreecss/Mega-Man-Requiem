#include "j1App.h"
#include "Crazy_Razy.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"

Crazy_Razy::Crazy_Razy(int x, int y) : Enemy(x, y)
{
	left.PushBack({ 146, 7, 32, 31 });
	left.PushBack({ 181, 7, 32, 31 });
	left.PushBack({ 216, 7, 32, 31 });
	left.speed = 0.2f;

	right.PushBack({ 147, 52, 32, 31 });
	right.PushBack({ 182, 52, 32, 31 });
	right.PushBack({ 217, 52, 32, 31 });
	right.speed = 0.2f;

	animation = &left;

	original_pos.y = y;
	original_pos.x = x;

	collider = App->collision->AddCollider({ 0, 0, 32, 31 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);
}

void Crazy_Razy::Move() {

}

void Crazy_Razy::OnCollision(Collider* collider) {

}