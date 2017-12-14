#include "j1App.h"
#include "Image.h"

Image::Image(iPoint screen_area, SDL_Rect image, const SDL_Texture* tex, bool is_draggable, j1Module* callback)
{
	type = IMAGE;
	this->screen_area = screen_area;
	this->tex = tex;
	this->is_draggable = is_draggable;
	this->callback = callback;
	UI_logic_rect = image;
}

void Image::Draw() {

	App->render->Blit(tex, screen_area.x, screen_area.y, &UI_logic_rect, 0.0f);

}
void Image::ChangeImage(SDL_Rect new_image) {

	UI_logic_rect = new_image;

}