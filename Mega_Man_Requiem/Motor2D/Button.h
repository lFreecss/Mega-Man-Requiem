#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "UI.h"


class Button : public UI {

private:
	SDL_Rect normal_img;
	SDL_Rect hover_img;
	SDL_Rect pressed_img;
public:

	Button(iPoint screen_area, SDL_Rect normal_img, SDL_Rect hover_img, SDL_Rect pressed_img, const SDL_Texture* tex, bool is_draggable, j1Module* callback);
	void Draw();
	void ChangeToNormalImg();
	void ChangeToHoverImg();
	void ChangeToPressedImg();
};

#endif