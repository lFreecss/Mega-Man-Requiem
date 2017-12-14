#include "j1App.h"
#include "j1Collision.h"
#include "Letter.h"

Letter::Letter(int x, int y) : Entity(x, y)
{
	idle.PushBack({ 6, 186, 16, 16 });
	idle.PushBack({ 25, 186, 16, 16 });
	animation = &idle;
	
	size = { 16,16 };
	collider = App->collision->AddCollider({ 0, 0, size.x, size.y }, COLLIDER_TYPE::COLLIDER_LETTER, (j1Module*)App->entities);

}

void Letter::UpdateAnim(float dt) {
	idle.speed = 1*dt;
}