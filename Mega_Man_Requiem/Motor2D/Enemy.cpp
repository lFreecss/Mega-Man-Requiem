#include "j1App.h"
#include "Enemy.h"
#include "j1Render.h"

Enemy::Enemy(int x, int y) : pos(x, y)
{}

Enemy::~Enemy()
{}

void Enemy::Draw(SDL_Texture* sprites)
{
	if (animation != nullptr)
		App->render->Blit(sprites, pos.x, pos.y, &(animation->GetCurrentFrame()));
}

void Enemy::Move()
{

}