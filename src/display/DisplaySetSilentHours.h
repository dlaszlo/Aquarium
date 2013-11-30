#ifndef DISPLAYSETSILENTHOURS_H_
#define DISPLAYSETSILENTHOURS_H_

#include "Display.h"

class DisplaySetSilentHours : public Display
{
public:
	DisplaySetSilentHours(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYSETSILENTHOURS_H_ */
