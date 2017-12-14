#include "j1App.h"
#include "Crazy_Razy.h"
#include "j1Collision.h"
#include "j1Pathfinding.h"
#include "j1Player.h"
#include "j1Map.h"
#include "j1Input.h"

Crazy_Razy::Crazy_Razy(int x, int y) : Entity(x, y)
{
	
	cr = App->entities->CRInfo();
	
	path = nullptr;

	left.PushBack({ 146, 7, 32, 31 });
	left.PushBack({ 181, 7, 32, 31 });
	left.PushBack({ 216, 7, 32, 31 });

	right.PushBack({ 147, 52, 32, 31 });
	right.PushBack({ 182, 52, 32, 31 });
	right.PushBack({ 217, 52, 32, 31 });

	animation = &left;

	original_pos.y = y;
	original_pos.x = x;

	collider = App->collision->AddCollider({ 0, 0, cr.size.x, cr.size.y }, COLLIDER_TYPE::COLLIDER_ENEMY, (j1Module*)App->entities);
}

void Crazy_Razy::UpdateAnim(float dt) {
	left.speed = cr.anim1_speed*dt;
	right.speed = cr.anim2_speed*dt;

}

void Crazy_Razy::Move(float dt) {
	player_pos.x = (int)App->player->pos.x;
	player_pos.y = (int)App->player->pos.y;
	
	if (pos == original_pos)
		iteration = 0;

	pos.y += cr.gravity * dt;;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + cr.size.x, pos.y + cr.size.y);
	if (App->map->CollisionY(posWorld.x, endPosWorld.x, posWorld.y + 2)) {
		pos.y -= cr.gravity * dt;
	}
	
	//It creates some kind of radar with barriers that the Entity cannot cross in case the player is outside this barriers. 
	//If the player is in this barriers creates a path.
	if (iteration == 0 && player_pos.x < original_pos.x + cr.ending_radar_limit) {
		if (player_pos.x < cr.positioning_barrier && player_pos.x > original_pos.x - player_pos.x + cr.radar_limit_1)
			CreatePath();
		else if (player_pos.x > cr.positioning_barrier && player_pos.x > original_pos.x - player_pos.x + cr.radar_limit_2)
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
		pos.x += cr.speed*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + cr.size.x, pos.y + cr.size.y);
		if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
			pos.x -= cr.speed*dt;
		animation = &right;
	}

	if (pos.x > next_pos.x) {
		pos.x -= cr.speed*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + cr.size.x, pos.y + cr.size.y);
		if (App->map->CollisionX(posWorld.x + 2, posWorld.y, endPosWorld.y))
			pos.x += cr.speed*dt;
		animation = &left;
	}

	if (pos.x == next_pos.x)
		iteration++;

	if (iteration == destination || player_pos.x < pos.x && iteration > 5) 
		iteration = 0;

}