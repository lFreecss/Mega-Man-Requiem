#include "j1App.h"
#include "Label.h"
#include "j1Fonts.h"
#include "j1Textures.h"


Label::Label(iPoint screen_area, p2SString text, _TTF_Font* font, SDL_Color colour, bool is_draggable, j1Module* callback) : text(text), font(font), colour(colour)
{
	type = LABEL;
	this->screen_area = screen_area;
	this->is_draggable = is_draggable;
	this->callback = callback;

	this->text = text;
	tex = App->font->Print(text.GetString(), colour, font);
	UI_logic_rect.x = 0;
	UI_logic_rect.y = 0;
	SDL_QueryTexture((SDL_Texture*)tex, NULL, NULL, &UI_logic_rect.w, &UI_logic_rect.h);

}

void Label::Draw() {
	App->render->Blit(tex, screen_area.x, screen_area.y, &UI_logic_rect, 0.0f);
}

void Label::ChangeText(p2SString text) {

	this->text = text;
	App->tex->UnLoad((SDL_Texture*)tex);
	tex = App->font->Print(text.GetString(), colour, font);

}

p2SString Label::GetText() const {
	return text;
}

void Label::ChangeColor(SDL_Color colour) {

	App->tex->UnLoad((SDL_Texture*)tex);
	tex = App->font->Print(text.GetString(), colour, font);

}