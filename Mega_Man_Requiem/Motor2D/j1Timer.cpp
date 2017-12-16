// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	// TODO 1: Fill Start(), Read(), ReadSec() methods
	// they are simple, one line each!
	started_at = SDL_GetTicks();
	seconds_paused = 0;
}

// To Load Time
void j1Timer::SetLoadTime(float transcurred_seconds) {
	seconds_paused = 0;
	started_at = SDL_GetTicks() - 1000.0f*transcurred_seconds;
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	return ((SDL_GetTicks() - started_at) / 1000.0f) - seconds_paused;
}

// To Pause Game
// ---------------------------------------------
void j1Timer::Pause()
{
	stop_at = SDL_GetTicks();
}

// ---------------------------------------------
void j1Timer::Continue()
{
	seconds_paused += (SDL_GetTicks() - stop_at) / 1000.0f;

}