#include "j1App.h"
#include "j1Collision.h"
#include "Letter.h"

Letter::Letter(int x, int y) : Entity(x, y)
{
	//MEGA MAN R letters
	idle.PushBack({ 6, 186, 16, 16 }); //M
	idle.PushBack({ 25, 186, 16, 16 }); //E
	idle.PushBack({ 44, 186, 16, 16 }); //G
	idle.PushBack({ 63, 186, 16, 16 }); //A
	idle.PushBack({ 6, 186, 16, 16 }); //M
	idle.PushBack({ 63, 186, 16, 16 }); //A
	idle.PushBack({ 82, 186, 16, 16 }); //N
	idle.PushBack({ 101, 186, 16, 16 }); //R
	animation = &idle;
	
	size = { 16,16 };
	collider = App->collision->AddCollider({ 0, 0, size.x, size.y }, COLLIDER_TYPE::COLLIDER_LETTER, (j1Module*)App->entities);

}

void Letter::UpdateAnim(float dt) {
	idle.speed = 1*dt;
}