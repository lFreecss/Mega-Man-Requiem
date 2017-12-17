#ifndef __LABEL_H__
#define __LABEL_H__

#include "p2SString.h"
#include "UI.h"
struct _TTF_Font;

class Label : public UI {

private:

	p2SString text;
	_TTF_Font* font = nullptr;
	SDL_Color colour;

public:

	Label(iPoint screen_area, p2SString text, _TTF_Font* font, SDL_Color colour, bool is_draggable, j1Module* callback);
	void Draw();
	void ChangeText(p2SString text);
	p2SString GetText() const;
	void ChangeColor(SDL_Color colour);

};


#endif