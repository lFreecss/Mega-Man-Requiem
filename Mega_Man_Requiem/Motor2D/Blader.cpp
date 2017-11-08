#include "j1App.h"
#include "Blader.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"

Blader::Blader(int x, int y) : Enemy(x, y)
{
	idle.PushBack({ 187, 108, 16, 21 });
	idle.PushBack({ 206, 108, 16, 21 });
	idle.speed = 0.2f;

	animation = &idle;

	original_pos.y = y;
	original_pos.x = x;

	collider = App->collision->AddCollider({ 0, 0, 16, 21 }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);
}

void Blader::Move() {
	//pos.x += 1;
	iPoint player_pos ;
	player_pos.x = -(int)App->player->pos.x;
	player_pos.y = -(int)App->player->pos.y;
	
	App->pathfinding->CreatePath(original_pos, player_pos);
	
	App->pathfinding->GetLastPath();
}

void Blader::OnCollision(Collider* collider) {



}