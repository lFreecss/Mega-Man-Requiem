#include "j1App.h"
#include "j1Collision.h"
#include "Letter.h"

Letter::Letter(int x, int y, SDL_Rect letr) : Entity(x, y)
{
	idle.PushBack(letr);
	animation = &idle;
	size = { 16,16 };

	collider = App->collision->AddCollider({ 0, 0, size.x, size.y }, COLLIDER_TYPE::COLLIDER_LETTER, (j1Module*)App->entities);

}