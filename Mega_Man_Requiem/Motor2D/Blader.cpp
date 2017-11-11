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
	iPoint player_pos;
	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;
	if (player_pos.x > original_pos.x - player_pos.x && player_pos.x < 500 && iteration == 0) {
		//if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
			destination = App->pathfinding->CreatePath(App->map->WorldToMap(pos.x, pos.y), App->map->WorldToMap(player_pos.x, player_pos.y));
			path = App->pathfinding->GetLastPath();
			iteration = 0;
		//}
	}

	if (path != nullptr && path->At(iteration) != nullptr) {
		iPoint next_pos = App->map->MapToWorld(path->At(iteration)->x, path->At(iteration)->y);
		if (pos.x < next_pos.x)
			pos.x++;
		if (pos.y < next_pos.y)
			pos.y++;
		if (pos.x > next_pos.x)
			pos.x--;
		if (pos.y > next_pos.y)
			pos.y--;

		if(pos.x == next_pos.x && pos.y == next_pos.y)
			iteration++;

		if (iteration == destination)
			iteration = 0;
		//pos.x = next_pos.x;
		//pos.y = next_pos.y;
		//pos = App->map->MapToWorld(next_pos.x, next_pos.y);
	}
}

void Blader::OnCollision(Collider* collider) {



}