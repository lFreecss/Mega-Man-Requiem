#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Input.h"
#include "j1map.h"
#include "j1Collision.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1FadeToBlack.h"
#include "Brofiler\Brofiler.h"


j1Player::j1Player() : j1Module() {
	name.create("player");
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 133, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });
	idle.PushBack({ 103, 10, 21, 24 });

	right.PushBack({ 188, 10, 24, 24 });
	right.PushBack({ 218, 10, 21, 24 });
	right.PushBack({ 239, 10, 21, 24 });

	left.PushBack({ 281, 86, 24, 24 });
	left.PushBack({ 259, 86, 21, 24 });
	left.PushBack({ 233, 86, 21, 24 });

	jumpR.PushBack({ 265, 4, 27, 30 });

	sJump.PushBack({ 206, 209, 26, 30 });
	sJump.PushBack({ 239, 209, 26, 30 });
	sJump.PushBack({ 266, 209, 26, 30 });
	sJump.PushBack({ 297, 209, 26, 30 });
	sJump.PushBack({ 206, 209, 26, 30 });

	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 103, 10, 21, 24 });
	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 103, 10, 21, 24 });
	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 133, 10, 21, 24 });
	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 103, 10, 21, 24 });
	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 103, 10, 21, 24 });
	idle_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	idle_inv.PushBack({ 103, 10, 21, 24 });

	right_inv.PushBack({ 188, 10, 24, 24 });
	right_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	right_inv.PushBack({ 218, 10, 21, 24 });
	right_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	right_inv.PushBack({ 239, 10, 21, 24 });

	left_inv.PushBack({ 281, 86, 24, 24 });
	left_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	left_inv.PushBack({ 259, 86, 21, 24 });
	left_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	left_inv.PushBack({ 233, 86, 21, 24 });

	jumpR_inv.PushBack({ 265, 4, 27, 30 });
	jumpR_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	jumpR_inv.PushBack({ 265, 4, 27, 30 });

	sJump_inv.PushBack({ 206, 209, 26, 30 });
	sJump_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	sJump_inv.PushBack({ 239, 209, 26, 30 });
	sJump_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	sJump_inv.PushBack({ 266, 209, 26, 30 });
	sJump_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	sJump_inv.PushBack({ 297, 209, 26, 30 });
	sJump_inv.PushBack({ 0, 240, 21, 24 }); //INV FRAME
	sJump_inv.PushBack({ 206, 209, 26, 30 });

}

j1Player::~j1Player()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

bool j1Player::Awake(pugi::xml_node& config) {
	bool ret = true;
	LOG("Loading Player");

	path.create(config.child("file").attribute("name").as_string());

	vel.x = config.child("physics").attribute("velocityx").as_float();
	vel.y = config.child("physics").attribute("velocityy").as_float();
	gravity = config.child("physics").attribute("gravity").as_float();
	floor_level = config.child("physics").attribute("floor_level").as_uint();

	jumpMaxTime = config.child("movement").attribute("jumpMaxTime").as_float();

	size.x = config.child("size").attribute("x").as_int();
	size.y = config.child("size").attribute("y").as_int();

	//Loading animation speeds
	pugi::xml_node animation = config.child("animation");
	idle_speed = animation.attribute("idle_speed").as_float();
	right_speed = animation.attribute("right_speed").as_float();
	left_speed = animation.attribute("left_speed").as_float();
	jumpR_speed = animation.attribute("jumpR_speed").as_float();
	sJump_speed = animation.attribute("sJump_speed").as_float();
	idle_inv_speed = animation.attribute("idle_inv_speed").as_float();
	right_inv_speed = animation.attribute("right_inv_speed").as_float();
	left_inv_speed = animation.attribute("left_inv_speed").as_float();
	jumpR_inv_speed = animation.attribute("jumpR_inv_speed").as_float();
	sJump_inv_speed = animation.attribute("sJump_inv_speed").as_float();

	return ret;
}

bool j1Player::Start() {
	bool ret = true;
	graphics = App->tex->Load(path.GetString());
	if (graphics == nullptr)
		ret = false;

	
	Init();

	return ret;
}

void j1Player::Init() {
	pos = startPos;
	App->collision->EraseCollider(collider);
	jumping = 1;
	actualJumpTime = 0;
	collider = App->collision->AddCollider({ (int)startPos.x, (int)startPos.y, 21, 24 }, COLLIDER_PLAYER, this);

}

void j1Player::updateAnim(float dt) {
	idle.speed = idle_speed*dt;
	right.speed = right_speed*dt;
	left.speed = left_speed*dt;
	jumpR.speed = jumpR_speed*dt;
	sJump.speed = sJump_speed*dt;
	idle_inv.speed = idle_inv_speed*dt;
	right_inv.speed = right_inv_speed*dt;
	left_inv.speed = left_inv_speed*dt;
	jumpR_inv.speed = jumpR_inv_speed*dt;
	sJump_inv.speed = sJump_inv_speed*dt;
}


bool j1Player::Update(float dt) {
	bool ret = true;
	

	if (lives == 0) {
		App->scene->GameOverScreen();
		lives = 3;
	}

	if (!App->isPaused) //Pause
	{
		current_animation = &idle;

		if (GodMode())
			current_animation = &idle_inv;

		move(dt);
		jump(dt);
	}
	updateAnim(dt);


		if (jumping == 1) {
			current_animation = &jumpR;
			if (GodMode()) {
				if (pos.y < floor_level)
					current_animation = &jumpR_inv;
				else
					current_animation = &idle_inv;
			}
		}
		if (jumping == 0) {
			current_animation = &sJump;
			if (GodMode())
				current_animation = &sJump_inv;
		}


		SDL_Rect r = current_animation->GetCurrentFrame();
		App->render->Blit(graphics, pos.x, pos.y, &r);

		if (collider != nullptr)
			collider->SetPos((int)pos.x, (int)pos.y);

	return ret;
}

bool j1Player::CleanUp() {
	bool ret = true;
	
	if (collider != nullptr) {
		App->collision->EraseCollider(collider);
		collider = nullptr;
	}
	App->tex->UnLoad(graphics);

	return ret;
}

void j1Player::jump(float dt) {
	
	if (actualJumpTime > 0) {
		actualJumpTime += dt;
		if (actualJumpTime > jumpMaxTime) {
			actualJumpTime = 0;
		}
		else {
			pos.y -= vel.y*dt;
			iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
			iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
			if (App->map->CollisionY(posWorld.x, endPosWorld.x, posWorld.y)) {
				pos.y += gravity*dt;
				
			}
		}
	}

	pos.y += gravity*dt;
	iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
	iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
	if (App->map->CollisionY(posWorld.x, endPosWorld.x, endPosWorld.y)) {
		pos.y -= gravity*dt;
		jumping = 2;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		landed = true;
		if (jumping > 0) {
			jumping --;
			actualJumpTime = dt;
		}
	}

	//Sound that Mega Man makes when he lands
	if (jumping == 2 && landed == true) {
		App->audio->PlayFx(1, 0);
		landed = false;
	}
}


void j1Player::move(float dt) {
	BROFILER_CATEGORY("Player_Move_Collisions", Profiler::Color::DarkOrange);
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		current_animation = &left;
		if(GodMode())
			current_animation = &left_inv;

		pos.x -= vel.x*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(posWorld.x, posWorld.y, endPosWorld.y)) {
			pos.x += vel.x*dt;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		current_animation = &right;
		if (GodMode())
			current_animation = &right_inv;

		pos.x += vel.x*dt;
		iPoint posWorld = App->map->WorldToMap(pos.x, pos.y);
		iPoint endPosWorld = App->map->WorldToMap(pos.x + size.x, pos.y + size.y);
		if (App->map->CollisionX(endPosWorld.x, posWorld.y, endPosWorld.y)) {
			pos.x -= vel.x*dt;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		current_animation = &idle;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	collider = c1;
	if (c2->type == COLLIDER_TYPE::COLLIDER_ENEMY && GodMode() == false) {
		App->collision->EraseCollider(collider);
		collider = nullptr;
		if (lives >= 0) {
			App->fade_to_black->FadeToBlack(App->scene->time_fade_to_black);
			lives--;
			App->audio->PlayFx(2, 0);
			App->scene->MapStart();
		}
	}
}

bool j1Player::GodMode() {
	bool ret = is_invincible;
	if (is_invincible == false)
		return ret;

		if (pos.y > floor_level) {
			pos.y = floor_level;
			jumping = 1;
		}

	return ret;
}

uint j1Player::GetLives() {
	return lives;
}

Collider* j1Player::GetCollider() {
	return collider;
}

// Load player position
bool j1Player::Load(pugi::xml_node& data)
{
	pos.x = data.child("position").attribute("x").as_float();
	pos.y = data.child("position").attribute("y").as_float();
	lives = data.child("lives").attribute("quantity").as_uint();

	return true;
}

// Save player position
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node position = data.append_child("position");

	position.append_attribute("x") = pos.x;
	position.append_attribute("y") = pos.y;

	pugi::xml_node life = data.append_child("lives");
	life.append_attribute("quantity") = lives;

	return true;
}