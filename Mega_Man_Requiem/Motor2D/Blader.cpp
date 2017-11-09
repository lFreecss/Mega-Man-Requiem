#include "j1App.h"
#include "Blader.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"

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
	iPoint player_pos;
	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		App->pathfinding->CreatePath(App->map->WorldToMap(pos.x, pos.y), App->map->WorldToMap(player_pos.x, player_pos.y));
		path = App->pathfinding->GetLastPath();
		i = 0;
	}


	if (path != nullptr && path->At(i) != nullptr) {
		iPoint next_pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);

		pos.x = next_pos.x;
		pos.y = next_pos.y;

		//pos = App->map->MapToWorld(next_pos.x, next_pos.y);

		i++;

	}
}

void Blader::OnCollision(Collider* collider) {



}