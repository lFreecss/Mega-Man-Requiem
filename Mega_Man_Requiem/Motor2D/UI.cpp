#include "UI.h"
#include "j1App.h"
#include "j1Input.h"

UI::UI()
{
}

UI:: ~UI() {}

void UI::Update(float dt) {
	if (this != nullptr) {
		Draw();
		HandleInput();
	}
}

void UI::Draw() {}

void UI::HandleInput()
{
	iPoint mouse_pos = { 0,0 };
	App->input->GetMousePosition(mouse_pos.x, mouse_pos.y);

	if ((mouse_pos.x > screen_area.x && mouse_pos.x < screen_area.x + UI_logic_rect.w) && (mouse_pos.y > screen_area.y && mouse_pos.y < screen_area.y + UI_logic_rect.h) && state != LEFT_MOUSE_PRESS && state != RIGHT_MOUSE_PRESS && has_entered == false) {
		state = MOUSE_ENTER;
		has_entered = true;
	}

	else if ((mouse_pos.x > screen_area.x && mouse_pos.x < screen_area.x + UI_logic_rect.w) && (mouse_pos.y > screen_area.y && mouse_pos.y < screen_area.y + UI_logic_rect.h) && state != LEFT_MOUSE_PRESS && state != RIGHT_MOUSE_PRESS && has_entered == true)
		state = MOUSE_INSIDE;

	else if (state == MOUSE_ENTER || state == MOUSE_INSIDE) {
		state = MOUSE_LEAVE;
		has_entered = false;
	}

	else if (state == MOUSE_LEAVE)
		state = NONE;

	if (state == MOUSE_INSIDE && App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		state = LEFT_MOUSE_PRESS;

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && state == LEFT_MOUSE_PRESS)
		state = MOUSE_LEAVE;

	if (state == LEFT_MOUSE_PRESS) {

		if (mouse_pos != aux_mouse && is_draggable == true) {
			iPoint mov_mouse = { 0,0 };
			App->input->GetMouseMotion(mov_mouse.x, mov_mouse.y);
			screen_area.x += mov_mouse.x;
			screen_area.y += mov_mouse.y;
			aux_mouse = mouse_pos;
		}
	}

	if (state == MOUSE_INSIDE && App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
		state = RIGHT_MOUSE_PRESS;

	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP && state == RIGHT_MOUSE_PRESS)
		state = MOUSE_LEAVE;

	this->callback->UIInteraction(this, state);
}

UI_TYPE UI::GetType() {
	return type;
}