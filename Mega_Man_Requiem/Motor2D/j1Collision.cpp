#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	name.create("collision");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_LETTER] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_LETTER] = false;

	matrix[COLLIDER_LETTER][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_LETTER][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_LETTER][COLLIDER_LETTER] = false;	
}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::Awake(pugi::xml_node&) {
	bool ret = true;

	return ret;
}

bool j1Collision::Start() {
	bool ret = true;

	return ret;
}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}

	DebugDraw();

	return true;
}

void j1Collision::DebugDraw()
{
	if (view_collision == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_PLAYER: // blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
				break;
			case COLLIDER_ENEMY: // red
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
				break;
			case COLLIDER_LETTER: // yellow
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 0, alpha);
				break;
		}
	}
}

// Called before quitting
bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Collision::EraseCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}

	return false;
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}