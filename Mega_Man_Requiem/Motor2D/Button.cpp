#include "j1App.h"
#include "Button.h"
#include "Image.h"
#include "Label.h"
#include "j1Input.h"


Button::Button(iPoint screen_area, SDL_Rect normal_img, SDL_Rect hover_img, SDL_Rect pressed_img, const SDL_Texture* tex, bool is_draggable, j1Module* callback) {
	type = BUTTON;
	this->screen_area = screen_area;
	this->normal_img = normal_img;
	this->hover_img = hover_img;
	this->pressed_img = pressed_img;
	this->tex = tex;
	this->is_draggable = is_draggable;
	this->callback = callback;

	UI_logic_rect = normal_img;
}

void Button::Draw() {
	App->render->Blit(tex, screen_area.x, screen_area.y, &UI_logic_rect);
}

void Button::ChangeToNormalImg() {

	UI_logic_rect = normal_img;

}

void Button::ChangeToHoverImg() {

	UI_logic_rect = hover_img;

}

void Button::ChangeToPressedImg() {

	UI_logic_rect = pressed_img;

}