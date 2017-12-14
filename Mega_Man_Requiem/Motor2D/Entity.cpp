#include "j1App.h"
#include "Entity.h"
#include "j1Render.h"
#include "j1Collision.h"

Entity::Entity(int x, int y) : pos(x, y)
{}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
		if (collider != nullptr)
			collider->SetPos(pos.x, pos.y);

		if (animation != nullptr)
			App->render->Blit(sprites, pos.x, pos.y, &(animation->GetCurrentFrame()));
}

void Entity::UpdateAnim(float dt) {

}

void Entity::Move(float dt)
{}

void Entity::OnCollision(Collider* collider)
{}

void Entity::Awake(pugi::xml_node& config)
{}

void Entity::CreatePath()
{}
void Entity::FollowPath(float dt)
{}

