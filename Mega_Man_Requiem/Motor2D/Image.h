#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "UI.h"

class Image : public UI {

private:

public:
	Image(iPoint screen_area, SDL_Rect image, const SDL_Texture* tex, bool is_draggable, j1Module* callback);
	void Draw();
	void ChangeImage(SDL_Rect new_image);
};



#endif