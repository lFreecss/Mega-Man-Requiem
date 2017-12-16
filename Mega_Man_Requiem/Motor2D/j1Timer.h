#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void Pause();
	void Continue();
	void SetLoadTime(float transcurred_seconds);

private:
	uint32	started_at;
	uint32	stop_at;
	float	seconds_paused;
};

#endif //__j1TIMER_H__

