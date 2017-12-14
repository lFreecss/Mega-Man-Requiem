#include "j1App.h"
#include "Blader.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1Map.h"


Blader::Blader(int x, int y) : Entity(x, y)
{
	b = App->entities->BInfo();

	path = nullptr;

	idle.PushBack({ 187, 108, 16, 21 });
	idle.PushBack({ 206, 108, 16, 21 });

	animation = &idle;
	
	original_pos.y = y;
	original_pos.x = x;

	collider = App->collision->AddCollider({ 0, 0, b.size.x, b.size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entities);
}

void Blader::UpdateAnim(float dt) {
	idle.speed = b.anim1_speed*dt;
}


void Blader::Move(float dt) {

	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;
	
	if (pos == original_pos)
		iteration = 0;

	//It creates some kind of radar with barriers that the Entity cannot cross in case the player is outside this barriers. 
	//If the player is in this barriers creates a path.
	if (iteration == 0 && player_pos.x < original_pos.x + b.ending_radar_limit) {
		if (player_pos.x < b.positioning_barrier && player_pos.x > original_pos.x - player_pos.x + b.radar_limit_1 )
			CreatePath();
		else if (player_pos.x > b.positioning_barrier && player_pos.x > original_pos.x - player_pos.x + b.radar_limit_2)
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
		pos.x += b.speed*dt;
	if (pos.x > next_pos.x)
		pos.x -= b.speed*dt;
	if (pos.y < next_pos.y)
		pos.y += b.speed*dt;
	if (pos.y > next_pos.y)
		pos.y -= b.speed*dt;

	if (pos.x == next_pos.x && pos.y == next_pos.y)
		iteration++;

	if (iteration == destination || iteration > 5) 
		iteration = 0;

}