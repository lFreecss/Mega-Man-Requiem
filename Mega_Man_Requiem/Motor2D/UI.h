#ifndef __UI_H__
#define __UI_H__

#include "j1Render.h"

enum UI_TYPE {
	TYPE_NONE,
	IMAGE,
	LABEL,
	BUTTON
};

enum BUTTON_EVENTS {

	NONE,
	MOUSE_ENTER,
	MOUSE_LEAVE,
	LEFT_MOUSE_PRESS,
	RIGHT_MOUSE_PRESS,

};


class UI {

protected:

	iPoint screen_area = { 0,0 };
	SDL_Rect UI_logic_rect;
	const SDL_Texture* tex = nullptr;
	UI_TYPE type = TYPE_NONE;
	j1Module* callback = nullptr;
	bool is_draggable = false;

public:
	UI();
	virtual ~UI();
	void Update(float dt);
	virtual void Draw();
	void HandleInput();
	UI_TYPE GetType();

private:

	BUTTON_EVENTS state = NONE;
	iPoint aux_mouse = { 0, 0 };

};


#endif