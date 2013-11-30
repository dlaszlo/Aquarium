#ifndef DISPLAYSETLIGHT_H_
#define DISPLAYSETLIGHT_H_

#include "Display.h"

class DisplaySetLight : public Display
{
public:
	DisplaySetLight(Context & _context) : Display(_context) {};
	void init();
	uint8_t action();
	void render();
	uint8_t get_id();
	void close();
};

#endif /* DISPLAYSETLIGHT_H_ */
