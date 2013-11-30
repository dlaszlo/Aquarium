#ifndef DISPLAYSETCLOCK_H_
#define DISPLAYSETCLOCK_H_

#include "Display.h"

class DisplaySetClock : public Display
{
public:
	DisplaySetClock(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYSETCLOCK_H_ */
