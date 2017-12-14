#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "UI.h"
#include "Label.h"
#include "Image.h"
#include "Button.h"

#define CURSOR_WIDTH 2


enum FONT_TYPE {
	NO_FONT,
	MEGA_MAN_2,
	MEGA_MAN_10_SIZE_8,
	MEGA_MAN_10_SIZE_12,
};



// TODO 1: Create your structure of classes

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	//Update
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	Image* CreateImage(iPoint pos, SDL_Rect image, const SDL_Texture* tex, bool is_draggable, j1Module* callback);
	Label* CreateLabel(iPoint pos, p2SString text, _TTF_Font* font, SDL_Color colour, bool is_draggable, j1Module* callback);
	Button* CreateButton(iPoint pos, SDL_Rect normal_img, SDL_Rect hover_img, SDL_Rect pressed_img, const SDL_Texture* tex, bool is_draggable, j1Module* callback);

	_TTF_Font* GetFont(FONT_TYPE font_name);

	bool debug_print = false;

private:

	p2List<UI*> UIElements;
	p2List<j1Module*> ModulesList;

	_TTF_Font* Mega_Man_2;
	_TTF_Font* Mega_Man_10_sz8;
	_TTF_Font* Mega_Man_10_sz12;
	bool isCalled = false;
};

#endif // __j1GUI_H__