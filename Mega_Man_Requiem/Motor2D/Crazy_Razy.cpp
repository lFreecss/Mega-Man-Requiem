#include "j1App.h"
#include "Crazy_Razy.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Input.h"

Crazy_Razy::Crazy_Razy(int x, int y) : Enemy(x, y)
{
	path = nullptr;

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
	size.x = 32;
	size.y = 31;

	collider = App->collision->AddCollider({ 0, 0, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);
}

void Crazy_Razy::Move(float dt) {
	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;

	pos.y += 200.0 * dt;;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
	if (App->map->CollisionY(posWorld.x, endPosWorld.x, posWorld.y + 2)) {
		pos.y -= 200.0 * dt;
	}
	/*if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
	pos.x -= 150.0*dt;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
	if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
	pos.x += 150.0*dt;
	animation = &left;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
	pos.x += 150.0*dt;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
	if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
	pos.x -= 150.0*dt;
	animation = &right;
	}
	*/
	if (iteration == 0 && player_pos.x < player_pos.x < original_pos.x + 200) {
		if (player_pos.x < 1000 && player_pos.x > original_pos.x - player_pos.x + 200)
			CreatePath();
		else if (player_pos.x > 1000 && player_pos.x > original_pos.x - player_pos.x + 1350)
			CreatePath();
	}

	if (path != nullptr && path->At(iteration) != nullptr )
		FollowPath(dt);

}

void Crazy_Razy::OnCollision(Collider* collider) {

}

void Crazy_Razy::CreatePath() {
	destination = App->pathfinding->CreatePath(App->map->WorldToMap(pos.x, pos.y), App->map->WorldToMap(player_pos.x, player_pos.y), GROUND);
	path = App->pathfinding->GetLastPath();
	iteration = 0;
}

void Crazy_Razy::FollowPath(float dt) {
	iPoint next_pos = App->map->MapToWorld(path->At(iteration)->x, path->At(iteration)->y);
	if (pos.x < next_pos.x) {
		pos.x += 100.0*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
			pos.x -= 100.0*dt;
		animation = &right;
	}
	if (pos.x > next_pos.x) {
		pos.x -= 100.0*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
			pos.x += 100.0*dt;
		animation = &left;
	}

	if (pos.x == next_pos.x)
		iteration++;

	if (iteration == destination || player_pos.x < pos.x && iteration > 10)
		iteration = 0;

}