#include "j1App.h"
#include "Enemy.h"
#include "j1Render.h"
#include "j1Collision.h"

Enemy::Enemy(int x, int y) : pos(x, y)
{}

Enemy::~Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Enemy::GetCollider() const
{
	return collider;
}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(pos.x, pos.y);

	if (animation != nullptr)
		App->render->Blit(sprites, pos.x, pos.y, &(animation->GetCurrentFrame()));
}

void Enemy::Move(float dt)
{}

void Enemy::OnCollision(Collider* collider)
{}

void Enemy::Awake(pugi::xml_node& config)
{}

void Enemy::CreatePath()
{}
void Enemy::FollowPath(float dt)
<<<<<<< HEAD
{}
=======
{}
>>>>>>> origin/master
