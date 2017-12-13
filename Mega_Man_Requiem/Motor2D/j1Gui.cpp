#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	Mega_Man_2 = App->font->Load("fonts/Mega_Man_2.ttf", 10);
	Mega_Man_10_sz8 = App->font->Load("fonts/Mega_Man_10.ttf", 8);
	Mega_Man_10_sz12 = App->font->Load("fonts/Mega_Man_10.ttf", 12);
	p2List_item<UI*>* elem = UIElements.start;
	while (elem != nullptr) {
		elem = nullptr;
	}

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::Update(float dt)
{
	bool ret = true;

	p2List_item<UI*>* elem = UIElements.start;
	while (elem != nullptr) {
		elem->data->Update(dt);
		elem = elem->next;
	}

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UI*>* elem = UIElements.start;
	while (elem != nullptr) {
		RELEASE(elem->data);
		elem = elem->next;
	}

	return true;
}


Image* j1Gui::CreateImage(iPoint pos, SDL_Rect image, const SDL_Texture* tex, bool is_draggable, j1Module* callback) {
	Image* ret = new Image(pos, image, tex, is_draggable, callback);
	UIElements.add(ret);
	return ret;
}

Label* j1Gui::CreateLabel(iPoint pos, p2SString text, _TTF_Font* font, SDL_Color colour, bool is_draggable, j1Module* callback) {
	Label* ret = new Label(pos, text, font, colour, is_draggable, callback);
	UIElements.add(ret);
	return ret;
}

Button* j1Gui::CreateButton(iPoint pos, SDL_Rect normal_img, SDL_Rect hover_img, SDL_Rect pressed_img, const SDL_Texture* tex, bool is_draggable, j1Module* callback) {
	Button* ret = new Button(pos, normal_img, hover_img, pressed_img, tex, is_draggable, callback);
	UIElements.add(ret);
	return ret;
}


_TTF_Font* j1Gui::GetFont(FONT_TYPE font_name) {
	switch (font_name) {
	case MEGA_MAN_2:
		return Mega_Man_2;
		break;
	case MEGA_MAN_10_SIZE_8:
		return Mega_Man_10_sz8;
		break;
	case MEGA_MAN_10_SIZE_12:
		return Mega_Man_10_sz12;
		break;
	default:
		return Mega_Man_2;
		break;
	}

}

// class Gui ---------------------------------------------------
