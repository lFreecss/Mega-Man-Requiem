#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1FadeToBlack : public j1Module
{
public:
	j1FadeToBlack();

	// Destructor
	virtual ~j1FadeToBlack();

	bool Start();
	bool Update(float dt);
	bool FadeToBlack(float time = 2.0f);
	bool IsFading() const;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	int screen_width = 0;
	int screen_height = 0;
	int screen_size = 0;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__j1FADETOBLACK_H__