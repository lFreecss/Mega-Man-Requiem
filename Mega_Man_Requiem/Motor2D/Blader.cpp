#include "j1App.h"
#include "Blader.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"

void Blader::Awake(pugi::xml_node& config) {
	size.x = config.child("blader").child("size").attribute("x").as_int();
	size.y = config.child("blader").child("size").attribute("y").as_int();
}

Blader::Blader(int x, int y) : Enemy(x, y)
{
	path = nullptr;

	idle.PushBack({ 187, 108, 16, 21 });
	idle.PushBack({ 206, 108, 16, 21 });
	idle.speed = 0.2f;

	animation = &idle;
	size.x = 16;
	size.y = 21;
	original_pos.y = y;
	original_pos.x = x;

	collider = App->collision->AddCollider({ 0, 0, size.x, size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->enemies);
}

void Blader::Move(float dt) {
	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;

	if (iteration == 0 && player_pos.x < player_pos.x < original_pos.x + 200) {
		if (player_pos.x < 700 && player_pos.x > original_pos.x - player_pos.x)
			CreatePath();
		else if (player_pos.x > 700 && player_pos.x > original_pos.x - player_pos.x + 1300)
			CreatePath();
	}

	if (path != nullptr && path->At(iteration) != nullptr)
		FollowPath(dt);

}

void Blader::OnCollision(Collider* collider) {



}

void Blader::CreatePath() {
	destination = App->pathfinding->CreatePath(App->map->WorldToMap(pos.x, pos.y), App->map->WorldToMap(player_pos.x, player_pos.y), AIR);
	path = App->pathfinding->GetLastPath();
	iteration = 0;
}

void Blader::FollowPath(float dt) {
	iPoint next_pos = App->map->MapToWorld(path->At(iteration)->x, path->At(iteration)->y);
	if (pos.x < next_pos.x)
		pos.x++;
	if (pos.y < next_pos.y)
		pos.y++;
	if (pos.x > next_pos.x)
		pos.x--;
	if (pos.y > next_pos.y)
		pos.y--;

	if (pos.x == next_pos.x && pos.y == next_pos.y)
		iteration++;

	if (iteration == destination || iteration > 10)
		iteration = 0;

}